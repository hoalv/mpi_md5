#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
using namespace std;
int n; 
char a[100];
char dictionary[] = {'a', 'b', 'c', 'd', 'e', 'g', '1', '2', '3','4', '5','6', '\0' };
bool check = false;
void display(int k){
 for (int i = 0; i < k; i++){
  cout << a[i];
 }
 cout << endl;
}
char* getTextValue(int k){
	char* temp_arr;
	temp_arr = new char[k];
	for (int i = 0; i < k; i++){
  		temp_arr[i] = a[i];
 	}
 	temp_arr[k] = '\0';
 	return temp_arr;
}
void nextString(int k){
 int i = k-1;
 while (a[i]==dictionary[n-1]&&i>=0){  // neu a[i] = n và i >= 0 thì giam i
  i--;
 }
 if (i == -1) check = true; // neu i = -1 thì dã den cau hình cuoi cùng 
 else
 {
  int curr_index = 0;
  for (int m = 0; m < n; m++){
  	if(dictionary[m] == a[i]){
  		curr_index = m;
  		break;
	}
 }
  a[i] = dictionary[curr_index+1];     // tang a[i] lên 1 don vi
  for (int j = i + 1; j < k; j++){
   a[j] = dictionary[0];   // dat tat ca phan tu phía sau a[i] = 1
  }

 }
}
string concat(char* arr, char ch, int k){
	//int len = sizeof(arr)/sizeof(*arr) - 1;
	char temp_arr[100];
	temp_arr[0] = ch;
	for (int i=0; i<k; i++){
		temp_arr[i+1] = arr[i];
	}
	temp_arr[k+1] = '\0';
	return temp_arr;
}

