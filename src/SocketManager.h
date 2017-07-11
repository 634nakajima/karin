#ifndef SocketManager_h
#define SocketManager_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>

// メソッドのポインタをイベントリスナとするためのテンプレートクラス
class ListenerBase {
 public:
  virtual void callback(int *d, int size) = 0;
};

template <class T>
class MemFuncListener : public ListenerBase{
 public:
  MemFuncListener(T* obj, void (T::*cbf)(int *, int)){
    m_obj = obj;
    m_cbf = cbf;
  };

  virtual void callback(int *d, int size){
    (m_obj->*m_cbf)(d, size);
  };
 private:
  T* m_obj;
  void (T::*m_cbf)(int *, int);
};

class SocketManager {
 public:
  SocketManager();
  ~SocketManager();

  volatile int active;//サーバ起動中を示すフラグ
  //コールバック用
      template <class T>
	void setCallback(T* obj, void (T::*cbf)(int *, int)){
	m_listener = new MemFuncListener<T>(obj, cbf);
      };
      void callback(int *d, int size){
	if(m_listener) m_listener->callback(d, size);
      }

 private:
      ListenerBase *m_listener;

      //サーバ用スレッド
      pthread_t thread;
      static void *threadFunction(void *data);
      void startServer();
      void stopServer();

      //データ送信用内部メソッド
      void sendData(void *d, int size, char *ip);
      void broadcast(void *d, int size);
};
#endif /* SocketManager_h */
