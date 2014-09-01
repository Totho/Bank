
#include <czmq.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>
#include <stdio.h>

using namespace std;
typedef unordered_map <string,int> BankType;
BankType bank;

void dispatch(zmsg_t* incmsg, zmsg_t* outmsg) {
  zmsg_print(incmsg);
  char *operation = zmsg_popstr(incmsg);
  char *user = zmsg_popstr(incmsg);

  
  if(strcmp(operation,"Add") == 0) { 
    if (bank.count(user) > 0) {
      zmsg_addstr(outmsg,"User allready exist!!!");
    } else {
      bank[user]=0;
      zmsg_addstr(outmsg,"User was created!!!");
    }  
  } else if (strcmp(operation,"Send") == 0) {
    if (bank.count(user) > 0) {
      int temp=bank[user];
      char *c = zmsg_popstr(incmsg);
      int cantidad=atoi(c);
      
      bank[user]=temp+cantidad;
      zmsg_addstr(outmsg,"Send successful!");
      }else{
        zmsg_addstr(outmsg, "User exist, sending failed!!!");
      }
  } else if (strcmp(operation,"Take") == 0) {
    if (bank.count(user) > 0) {
      int temp=bank[user]; //saldo actual del usuario
      char *c = zmsg_popstr(incmsg);
      int cantidad=atoi(c);

      if(cantidad<=temp){
        bank[user]=temp-cantidad;
        zmsg_addstr(outmsg, "Successful!");
      }else{
        zmsg_addstr(outmsg, "Failed, No funds!");
      }
    }else{
        zmsg_addstr(outmsg, "User does not exist!!!");
    }
  }else if (strcmp(operation,"Transfer") == 0) {
    char *user2= zmsg_popstr(incmsg);
    if (bank.count(user) > 0 && bank.count(user2)>0) {
      int temp=bank[user]; //Saldo del usuario1
      int temp2=bank[user2]; //Saldo del usuario2
      char *c = zmsg_popstr(incmsg);
      int cantidad=atoi(c);
      

      if(cantidad<=temp){
        bank[user]=temp-cantidad;
        bank[user2]=temp2+cantidad;
        zmsg_addstr(outmsg, "Transfer successful!");
      }else{
        zmsg_addstr(outmsg, "Failed transfer, No Funds!!!");
      }
    }else{
        zmsg_addstr(outmsg, "Failed Trasnfer, verify Users!!!");
    }
  }else if (strcmp(operation,"Query") == 0) {

    if (bank.count(user) > 0) {
      int temp=bank[user];
      char cantidad[256];
      
      sprintf(cantidad,"%d",temp);
      
      zmsg_addstr(outmsg,cantidad);
      }else{
        zmsg_addstr(outmsg, "The user does not exist!!");
      }
  }else {
    zmsg_addstr(outmsg, "Error!!");
  }
}


int main (void)
{
  zctx_t *context = zctx_new();
  void *responder = zsocket_new(context, ZMQ_REP);
  zsocket_bind(responder, "tcp://*:5555");
  
  while (1) {
    zmsg_t* request = zmsg_recv(responder);
    zmsg_t *response = zmsg_new();
    dispatch(request,response);
    zmsg_send(&response,responder);
    }
    
    zctx_destroy(&context);
    return 0;
}
