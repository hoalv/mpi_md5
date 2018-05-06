# mpi_md5
#input
- edit "dictionary[]" (an array of char) in sinh_password.cpp
- edit "k" as length of string (plaintext) in function main() in file main.cpp
- edit md5 code that you want to find it's plaintext by change value of "code" in function main() in main.cpp

#run
-open terminal and cd to the folder you save your code, typing:
 + mpic++ main.cpp -o main                       
 + mpirun -np "number of process you want" main
  ex: mpirun -np 10 main
