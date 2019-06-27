# 线程总结

## 0.进程和线程对比

- 进程有自己的虚拟地址空间，线程没有
- 进程包含的内容有：地址空间、全局变量、打开的文件、子进程、定时器、信号和信号处理程序等
- 线程包含的东西有：程序计数器、寄存器、栈和状态(条件码)。每个线程都有自己的栈，但是堆是共享的。
- 多个线程运行在单一进程的上下文中，共享进程虚拟地址空间的所有内容，包括代码，数据，堆，共享库和打开的文件等。

## 1. 线程ID

线程ID的类型是`pthread_t`，它是一个结构体。《CASPP》上说线程ID是一个整数，其实仅限于Linux系统，不同的系统采用不同的数据类型来表示线程ID。

|系统|线程ID数据类型|
|---|---|
|linux 3.2.0|unsigned long|
|Solaris 10| unsigned int|
|FreeBSD 8.0|pthread*|
|Mac OS X 10.6.8|pthread*|

线程可以使用`pthread_self`函数获取自己的ID。

```C
pthread_t pthread_self(void);
```

判断两个线程ID是否相等，需要使用`pthread_equal`函数。

```C
//相等返回非0, 不等返回0
int pthread_equal(pthread_t tid1, pthread_t tid2);
```

## 2.线程创建

`pthread_create`函数用于创建线程。

```C
int pthread_create(pthread_t *tidp, const pthread_attr_t *attr,
                    (void*)(*start_rtn)(void*),void *arg);
```

- tidp:新创建的线程，其ID会被写入tidp指向的内存区域
- attr:设置线程属性，不需要的话设置为NULL
- start_rtn:新线程从`start_rtn`函数开始执行,start_rtn函数原型为
  
```C
void *start_rtn(void*);
```

如果需要向start_rtn函数传递多个参数，可以将参数包装成一个结构体，通过`arg`向start_rtn传该结构体指针。
线程创建例程参见`1.c`。

## 3.线程终止

如果任意线程调用了`exit`、`_Exit`或者`_exit`，那么整个进程就会终止。单个线程终止的方式有三种：

- 从`start_rtn`函数中return.
- 线程调用`pthread_exit`函数
- 线程被同一进程的其他进程cancel。

```C
void pthread_exit(void* rval_ptr);
int pthread_join(pthread_t thread, void** rval_ptr);
```

`ravl_ptr`是一个void指针，进程中的其他线程可以通过`pthread_join`函数访问到这个指针。
`pthread_join`将一直阻塞直到指定线程终止。

线程终止例子参见`2.c, 3.c`。

## 4.线程清理

线程可以在退出时执行需要调用的一些清理函数。一个线程可以注册多个清理函数，这些函数记录在栈中。执行顺序与注册顺序相反。

```C
void pthread_cleanup_push(void (*rtn)(void *), void *arg);
void pthread_clean_pop(int execute);
```

清理程序在以下三种情况会被执行

- 调用`pthread_exit`
- 响应`cancel`
- 用非零参数调用`pthread_cleanup_pop`函数

注意:从`start_rtn`函数`return`不会调用清理函数。例子参见`4.c`

## 5. 线程同步

当多个线程可以读取和修改同一个变量时(这个变量可能是全局变量或者是在堆上创建的变量，局部变量属于线程私有，不存在同步问题)，需要对线程进行同步，确保在访问变量的存储内容时不会访问到无效的值。线程不同步造成的问题参见例子`6.c`。

## 6. 互斥锁

可以使用`pthread`的互斥接口来保护数据，确保同一时间只有一个线程访问数据。

```C
// mutex init
int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
//mutex destory
int pthread_destory(pthread_mutex_t *mutex);
// lock mutex
int pthread_mutex_lock(pthread_mutex_t *mutex);
// unlock mutex
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

使用mutex保护数据实例参见`5.c 7.c`。

## 7.死锁
死锁产生的原因有:
1. 线程对同一个互斥锁加锁两次，那么它自身就会陷入死锁状态。
2. 程序中使用了一个以上的互斥锁，某一个线程A一直占有第一个互斥锁，并且在试图锁住第二个互斥量时处于阻塞状态(也就是说第二个锁被另外一个线程B占有)，但是线程B也在试图锁住第一个互斥锁。两个线程都在相互请求另一个线程拥有的资源，那么两个线程都没有办法向前执行，产生死锁。

下面这段代码就是造成死锁的典型例子(完整代码参见`10.c`)
```C
// thread A
void *write(void *temp)
{
    ...
    pthread_mutex_lock(&write_mutex);
    sleep(1);
    pthread_mutex_lock(&read_mutex);
    ...
    pthread_mutex_unlock(&read_mutex);
    pthread_mutex_unlock(&write_mutex);
    return ((void*)0);
}

// thread B
void *read(void *temp)
{
    ...
    pthread_mutex_lock(&read_mutex);
    sleep(1);
    pthread_mutex_lock(&write_mutex);
    ...
    pthread_mutex_unlock(&write_mutex);
    pthread_mutex_unlock(&read_mutex);
    return ((void*)0);
}
```

线程A执行写文件，首先锁住`write_mutex`，然后试图锁住`read_mutex`。但是`read_mutex`很有可能已经被线程b占用，于是两个线程各抱一个锁，互不撒手，产生死锁。

## 8. 避免死锁的方法
避免死锁有两种办法：
**方法1：** 仔细控制互斥量的加锁顺序。保证每个线程都按照同样的顺序对互斥量加锁，就可以避免死锁问题。
比如在上面的例子当中，写线程首先抱住`write_mutex`，然后试图抱住`read_mutex`，如果读线程也已相同的顺序占有锁，那么在写线程抱住`write_mutex`的时候，由于读线程抢不到`write_mutex`,因此也不可能对`read_mutex`加锁。这样写线程就可以同时抱住两把锁，线程得以正常执行。反过来，读线程先抢到`write_mutex`也是一样的道理。我们把上面的例程改一下就可以正常运行了。参见`10.c`。

```C
// thread A
void *write(void *temp)
{
    ...
    pthread_mutex_lock(&write_mutex);
    sleep(1);
    pthread_mutex_lock(&read_mutex);
    ...
    pthread_mutex_unlock(&read_mutex);
    pthread_mutex_unlock(&write_mutex);
    return ((void*)0);
}

// thread B
void *read(void *temp)
{
    ...
    pthread_mutex_lock(&write_mutex);
    sleep(1);
    pthread_mutex_lock(&read_mutex);
    ...
    pthread_mutex_unlock(&write_mutex);
    pthread_mutex_unlock(&read_mutex);
    return ((void*)0);
}
```

有的时候，程序的结构比较复杂，如果用到了太多的锁和数据结构，那么对互斥量排序是很困难的，那么就需要采用另外的方法。

**方法2：**使用`pthread`提供的`pthread_mutex_trylock`接口，如果已经占有某些锁的线程调用`pthread_mutex_trylock`接口返回成功，那么线程可以继续往前执行，如果不能获得锁，可以先释放自己占有的锁做好清理工作，待会儿再试试。

## 8.读写锁

互斥锁可以解决线程不同步的问题，但是带来了性能损失。假设有10000个线程，其中9999个线程只需要读一个共享变量，仅有一个线程需要修改该变量。那么在某一个线程读的时候，其他的9998个线程也只能阻塞，等待互斥锁释放，这显然是不合理的。读写锁的引入就是为了解决这个问题，提高读的并发能力。读写锁存在三种情况：

- 写线程拿到了读写锁， 那么此时锁处于写模式的加锁状态，其他线程只能阻塞等待。
- 只读线程拿到了读写锁，那么此时锁处于读模式的加锁状态。会出现两种情况：
  
  a) 在没有写线程阻塞的情况下，其他读线程可以继续加读模式的锁。
  
  b) 有写线程阻塞的情况下，读写锁通常会阻塞随后的读模式加锁。这样可以避免读模式锁长期占用，写线程被饿死。

读写锁的使用参见`9.c`

## 9.条件变量

