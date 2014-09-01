#include <czmq.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
 
  zctx_t *context = zctx_new();
  void *requester = zsocket_new(context, ZMQ_REQ);
  zsocket_connect(requester, "tcp://localhost:5555");


  zmsg_t* request = zmsg_new();
  
  if (strcmp(argv[1],"add") == 0) {
    zmsg_addstr(request,"Add");
    zmsg_addstr(request,argv[2]);
  } else if (strcmp(argv[1],"send") == 0) {
    zmsg_addstr(request,"Send");
    zmsg_addstr(request,argv[2]);
    zmsg_addstr(request,argv[3]);
  }else if (strcmp(argv[1],"take") == 0) {
    zmsg_addstr(request,"Take");
    zmsg_addstr(request,argv[2]);
    zmsg_addstr(request,argv[3]);
  }else if (strcmp(argv[1],"transfer") == 0) {
    zmsg_addstr(request,"Transfer");
    zmsg_addstr(request,argv[2]); //usuario que realizara la transaccion
    zmsg_addstr(request,argv[3]); //usuario destino de la transaccion
    zmsg_addstr(request,argv[4]); //Monto de la transaccion
  }else if (strcmp(argv[1],"query") == 0) {
    zmsg_addstr(request,"Query");
    zmsg_addstr(request,argv[2]); 
  }else {
    cout << "error!\n";
  } 
  
 
  zmsg_send(&request,requester);
  
  zmsg_t* resp = zmsg_recv(requester);
  
  zmsg_print(resp);      
         
  zctx_destroy(&context);
 
  return 0;
}

