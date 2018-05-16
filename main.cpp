#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#include "md5.cpp"
#include "sinh_password.cpp"

using std::cout; using std::endl;
 
 //tao chuoi password va kiem tra xem co phai password dung khong
string buildPlaintext (char char_start, string code, int k){
 n = sizeof(dictionary)/sizeof(*dictionary) - 1;				//do dai tu dien
 k--;
 for (int i = 0; i < k; i++){
  a[i] = dictionary[0];								//khoi tao chuoi password
 }
 check = false;
 while (!check){								//neu chua den cau hinh password cuoi cung
  char *plaintext = getTextValue(k);
  string str_plain = concat(plaintext, char_start, k);				//noi them vao dau chuoi mot ki tu ma tien trinh quan ly
  //cout << str_plain << endl;
  string md5code =  md5(str_plain);						// ma hoa pass theo md5 de so sanh
  if(md5code==code ){
	return str_plain;
  }
  nextString(k);								//sinh cau hinh password tiep theo
 }
 return "";
}
#define NE	20
#define KEY	30
#define DATA 	40
#define RESULT	60
int rank0(string code, int k ){
	int tne; 									// so ky tu trong tu dien;
	int np;										// tong so tien trinh
	int sne;									// so phan tu gui den cho moi tien trinh
	char *result;									// password ket qua
	char *tmp;
	string kq= "";
	string empty = "";		
	int i;
	MPI_Status status;
	/* lay ve so tien trinh */
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	/* tinh so phan tu gui cho moi tien trinh */
	tne = sizeof(dictionary)/sizeof(*dictionary) - 1;
	sne = tne / np;
	int remain = tne - sne*np;							//so ki tu du ra do khong chia het
	/* Gui du lieu cho cac tien trinh con*/
	for (i=1 ; i<np ; i++ ){
    	   MPI_Send(&sne, 1, MPI_INT, i, NE, MPI_COMM_WORLD);
	   MPI_Send(dictionary + i*sne,sne, MPI_CHAR, i, DATA, MPI_COMM_WORLD);
	}

	result = new char[k+1];
	tmp = new char[k+1];
	strcpy (result, empty.c_str());
	/* duyet password tren mien con ma no dam nhiem*/
	for(i=0; i<sne; i++){
		kq = buildPlaintext(dictionary[i], code, k);
		if(kq!=""){
			strcpy (result, kq.c_str());
			break;
		}
	}
	/*cout << remain << endl;
	if(remain>0){
	    for(i=sne*np; i<tne; i++){
		kq = buildPlaintext(dictionary[i], code, k);
		if(kq!=""){
			strcpy (result, kq.c_str());
			break;
		}
	    }
	}*/
	/* nhan du lieu tu cac tien trinh con gui ve */
	for ( i=1 ; i<np ; i++ ){
		MPI_Recv(tmp, k+1 , MPI_CHAR, i, RESULT, MPI_COMM_WORLD, &status);
		if(tmp != empty){
			result = tmp;
			break;
		}
	}
	/* thong bao ket qua */
	if(result != empty){
		cout << "Password do duoc la: " << result <<endl;
	}else{
		
		cout << "Khong tim thay.";
	}
	return 0;
}
int ranki(string code, int k){
	int ne;		// so phan tu cua mang con
	char *buff;	// mang con
	string kq = "";
	string empty = "";
	char *result;
	MPI_Status status;

	/* Nhan du lieu tu tien trinh 0 */
	MPI_Recv(&ne, 1, MPI_INT, 0, NE, MPI_COMM_WORLD, &status);
	buff = new char[ne+1];
	MPI_Recv(buff, ne, MPI_CHAR, 0, DATA, MPI_COMM_WORLD, &status);
	
	/* thuc hien thao tac duyet password */
	result = new char[k+1];
	strcpy (result, empty.c_str());
	for(int i=0; i<ne; i++){
		kq = buildPlaintext(buff[i], code, k);
		if(kq!=""){
			strcpy (result, kq.c_str());
			break;
		}
	}
	/* gui ket qua ve cho tien trinh goc */
	MPI_Send(result, k+1 , MPI_CHAR, 0, RESULT, MPI_COMM_WORLD);
	return 0;
}

int main(int argc, char ** argv){
	int rank,k;
	double time1, time2,duration,global;
	//cout <<"Nhap k: ";
	//cin >> k;
	k=6;
	string code = md5("999999");
	MPI_Init(&argc, &argv);
 	
	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	double start = MPI_Wtime();
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
		rank0(code, k);
	else
		ranki(code, k);
	
	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	double end = MPI_Wtime();
   		
	MPI_Finalize();

	if (rank == 0) { /* use time on master node */
    		printf("Runtime = %f\n", end-start);
	}
	return 0;
}


