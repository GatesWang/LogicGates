#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

typedef struct numst {
    char name[80];
    int value;
    struct numst * next;
}
numst;
typedef struct gatest {
  char operator[15];
  char operand1[30];
  char operand2[30];
  char operand3[30];
  char operand4[30];
  struct gatest * next;
}
gatest;
numst * addNum(numst * head, char * name) {
	numst * new;
  if (head == NULL) {
  	new = malloc(sizeof(numst));
    strcpy(new->name, name);
    new->next = NULL;
    return new;
  }

  numst * currentNum = head;
  while (currentNum->next != NULL) {
  	currentNum = currentNum->next;
  }
  new = malloc(sizeof(numst));
  strcpy(new->name, name);
  new->next = NULL;
  currentNum->next = new;
  return head;
}
gatest * addGate(gatest * head, char * operator, char * operand1, char * operand2, char * operand3, char * operand4) {
	gatest * new;
	if (head == NULL) {
		new = malloc(sizeof(gatest));
    strcpy(new->operator, operator);
    strcpy(new->operand1, operand1);
    strcpy(new->operand2, operand2);
    strcpy(new->operand3, operand3);
    strcpy(new->operand4, operand4);
    new->next = NULL;
    return new;
  }

  gatest * currentGate = head;
  while (currentGate->next != NULL) {
  	currentGate = currentGate->next;
  }

  new = malloc(sizeof(gatest));
  strcpy(new->operator, operator);
  strcpy(new->operand1, operand1);
  strcpy(new->operand2, operand2);
  strcpy(new->operand3, operand3);
  strcpy(new->operand4, operand4);
	currentGate->next = new;
  return head;
}
int getNum(char * name, numst * head1) {
  numst * ptr = head1;
  while (ptr != NULL) {
    if (strcmp(ptr->name, name) == 0) {
    	return ptr->value;
    }
    ptr = ptr->next;
  }
  return -1;
}
int setNum(char * name, int n, numst * head1) {
  numst * ptr = head1;
  while (ptr != NULL) {
    if (strcmp(ptr->name, name) == 0) {
    	ptr->value = n;
    	return 0;
		}
    ptr = ptr->next;
  }
  return -1;
}
int not(int a) {
  if (a == 1) {
    return 0;
  }
  else{
    return 1;
  }
}
int and(int a, int b) {
  if (a == 1 && b == 1) {
    return 1;
  }
  else{
  	return 0;
  }
}
int or(int a, int b) {
  if (a == 1 || b == 1) {
    return 1;
  }
  else{
  	return 0;
  }
}
int nand(int a, int b) {
  if (a == 1 && b == 1) {
    return 0;
  }
  else{
    return 1;
  }
}
int nor(int a, int b) {
  if (a == 1 || b == 1) {
    return 0;
  }
  else{
    return 1;
  }
}
int xor(int a, int b) {
  if (a == 1 && b == 0) {
    return 1;
  }
  else if (a == 0 && b == 1) {
    return 1;
  }
  else {
    return 0;
  }
}
void printArray(int ** array, int row, int separator, int col) {
  int i, j;
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++) {
			if(j==separator){
		  	printf("| %d ", array[i][j]);
			}
			else if(j==col-1){
				printf("%d", array[i][j]);
			}
			else{
				printf("%d", array[i][j]);
			}
    }
		printf("\n");
  }
}
int main(int argc, char ** argv) {
  FILE * fp;
  int m;
  int n;
  int numRows;
  int num1;
  int num2;
  int num3;
  int num4;
  int i;
  int j;
  int k;
  numst * inputHead = NULL; //a linked list containing the current input values
  numst * outputHead = NULL;//a linked list containing the current output values
  numst * tempHead = NULL;//a linked list containing the current temp values
  char * value = malloc(15 * sizeof(char));

  //get m and n
  fp = fopen(argv[1], "r");
  fscanf(fp, "INPUT %d", &m);
  for (i = 0; i < m; i++) {
    fscanf(fp, " %s", value);
    if(value[0] == ':'){
      fscanf(fp, " %s", value);
    }
    inputHead = addNum(inputHead, value);
  }

  fscanf(fp, "\n");

  fscanf(fp, "OUTPUT %d", &n);
  for (i = 0; i < n; i++) {
    fscanf(fp, " %s", value);
    if(value[0] == ':'){
      fscanf(fp, " %s", value);
    }
    outputHead = addNum(outputHead, value);
  }
  fscanf(fp, "\n");

  //create the array
  numRows = 1<<(m);
  int** array=malloc(numRows*sizeof(int*));
  for(i=0;i<numRows;i++){
    array[i]=(int*)malloc((m+n+1)*sizeof(int));
  }
  for(i=0; i<numRows; i++){
		unsigned mask = 1<<(m-1);
		char * strI = (char*) malloc((m+1)*sizeof(char));
		while (mask)
		{
			sprintf(strI, "%s%d", strI, ((mask & i) != 0));
			mask >>= 1;
		}
		for(j=0; j<m; j++){
			array[i][j] = strI[j] - '0';
		}
  }

  char * operator = malloc(30 * sizeof(char));
  char * operand1 = malloc(30 * sizeof(char));
  char * operand2 = malloc(30 * sizeof(char));
  char * operand3 = malloc(30 * sizeof(char));
  char * operand4 = malloc(30 * sizeof(char));

  //create gate structures
  gatest * gate = NULL;
  while (fscanf(fp, " %s", operator) == 1) {
  	if (strcmp("NOT", operator)==0 || strcmp("PASS", operator)==0) {
      fscanf(fp, " %s", operand1);
      if(operand1[0] == ':'){
        fscanf(fp, " %s", operand1);
      }
      if(getNum(operand1, inputHead)==-1 && getNum(operand1, outputHead)==-1){//if not an input or output variable
      	if(operand1[0] >= '0' && operand1[0] <= '9'){
      		//dont add it to anything
      	}
      	else if(getNum(operand1, tempHead) == -1){//not already stored
        	tempHead = addNum(tempHead, operand1);
      	}
      }
      fscanf(fp, " %s", operand2);
      if(operand2[0] == ':'){
        fscanf(fp, " %s", operand2);
      }
      if(getNum(operand2, inputHead)==-1 && getNum(operand2, outputHead)==-1){//if not an input or output variable
      	if(operand2[0] >= '0' && operand2[0] <= '9'){
      		//dont add it to anything
      	}
      	else if(getNum(operand2, tempHead) == -1){ //not already stored temp variable
        	tempHead = addNum(tempHead, operand2);
      	}
      }
    }
    else if (operator[0] == 'M') {//multiplexer
    	fscanf(fp, " %s", operand1);
    	int numSelectors = atoi(operand1);

    	char * temp = malloc(30 * sizeof(char));
      fscanf(fp, " %s", operand2);
      if(operand2[0] == ':'){// ignore colons
        fscanf(fp, " %s", operand2);
      }

      //store the input variables
      for (i = 0; i<(1<<numSelectors)-1; i++) {//each selector
        fscanf(fp, " %s", temp);
  			sprintf(operand2, "%s %s", operand2, temp);
      }
      //selectors
      fscanf(fp, " %s", operand3);
      if(operand3[0] == ':'){// ignore colons
        fscanf(fp, " %s", operand3);
      }
      //store the selector variables
      for (i = 0; i<numSelectors-1; i++) {//each selector
        fscanf(fp, " %s", temp);
  			sprintf(operand3, "%s %s", operand3, temp);
      }
      //output variable
      fscanf(fp, " %s", operand4);
      if(operand4[0] == ':'){// ignore colons
        fscanf(fp, " %s", operand4);
      }
      if(getNum(operand4, inputHead)==-1 && getNum(operand4, outputHead)==-1){//if not an input or output variable
      	if(getNum(operand4, tempHead) == -1){//it not a stored temp variable
      		tempHead = addNum(tempHead, operand4);
      	}
      }
    }
    else if (operator[0] == 'D') {//decoder
    	fscanf(fp, " %s", operand1);//this is the number of inputs
    	int numInputs = atoi(operand1);

    	char * temp = malloc(30 * sizeof(char));
      fscanf(fp, " %s", operand2);
      if(operand2[0] == ':'){// ignore colons
        fscanf(fp, " %s", operand2);
      }
      //store the input variables
      for (i = 0; i<numInputs-1; i++) {//each selector
        fscanf(fp, " %s", temp);
  			sprintf(operand2, "%s %s", operand2, temp);
      }
      //store the outputs
      fscanf(fp, " %s", operand3);
      if(operand3[0] == ':'){// ignore colons
        fscanf(fp, " %s", operand3);
      }
      if(getNum(operand3, inputHead)==-1 && getNum(operand3, outputHead)==-1){//if not an input or output variable
				if(getNum(operand3, tempHead) == -1){//it not a stored temp variable
					tempHead = addNum(tempHead, operand3);
				}
			}
      //store the output variables
      for (i = 0; i<(1<<numInputs)-1; i++) {//each output
        fscanf(fp, " %s", temp);
        if(getNum(temp, inputHead)==-1 && getNum(temp, outputHead)==-1){//if not an input or output variable
        	if(getNum(temp, tempHead) == -1){//it not a stored temp variable
        		tempHead = addNum(tempHead, temp);
        	}
        }
  			sprintf(operand3, "%s %s", operand3, temp);
      }
      if(getNum(operand3, inputHead)==-1 && getNum(operand4, outputHead)==-1){//if not an input or output variable
      	if(getNum(operand4, tempHead) == -1){//it not a stored temp variable
      		tempHead = addNum(tempHead, operand4);
      	}
      }
    }
    else{
      fscanf(fp, " %s", operand1);
      if(operand1[0] == ':'){
        fscanf(fp, " %s", operand1);
      }
      if(getNum(operand1, inputHead)==-1 && getNum(operand1, outputHead)==-1){//if not an input or output variable
      	if(operand1[0] >= '0' && operand1[0] <= '9'){
      		//dont add it to anything
      	}
      	else if(getNum(operand1, tempHead) == -1){//not already stored
        	tempHead = addNum(tempHead, operand1);
      	}
      }
      fscanf(fp, " %s", operand2);
      if(operand2[0] == ':'){
        fscanf(fp, " %s", operand2);
      }
      if(getNum(operand2, inputHead)==-1 && getNum(operand2, outputHead)==-1){//if not an input or output variable
      	if(operand1[0] >= '0' && operand1[0] <= '9'){
      		//dont add it to anything
      	}
      	else if(getNum(operand2, tempHead) == -1){//not already stored
        	tempHead = addNum(tempHead, operand2);
      	}
      }
      fscanf(fp, " %s", operand3);
      if(operand3[0] == ':'){
        fscanf(fp, " %s", operand3);
      }
      if(getNum(operand3, inputHead)==-1 && getNum(operand3, outputHead)==-1){//if not an input or output variable
      	if(operand1[0] >= '0' && operand1[0] <= '9'){
      		//dont add it to anything
      	}
      	else if(getNum(operand3, tempHead) == -1){//not already stored
        	tempHead = addNum(tempHead, operand3);
      	}
      }
    }
		//printf("%s %s\n", operator, operand4);
    gate = addGate(gate, operator, operand1, operand2, operand3, operand4);
    fscanf(fp, "\n");
  }
  printf("\n");
  //start getting outputs
  for (i = 0; i < numRows; i++) {
    numst * inputPtr = inputHead; //a pointer in the input list
  	//set inputs
    for (j = 0; j < m; j++) {
    	inputPtr->value = array[i][j];
    	inputPtr = inputPtr->next;
    }
    gatest * currentGate = gate;
		while (currentGate != NULL) {
			//not gate
			if (currentGate->operator[0] == 'N' && currentGate->operator[2] == 'T') {
				//get
				num1 = getNum(currentGate->operand1, inputHead);
				if(currentGate->operand1[0] >= '0' && currentGate->operand1[0] <= '9'){//it is a number
					//dont get the value from a list
					num1 = operand1[0] - '0';
				}
				else if(num1==-1){//it is a temp variable
					num1 = getNum(currentGate->operand1, tempHead);
				}
				//operate
				num2 = not(num1);
				//set
				num1 = setNum(currentGate->operand2, num2, outputHead);
				if(num1==-1){//it is a temp variable
					setNum(currentGate->operand2, num2, tempHead);
				}
			}
      else if (currentGate->operator[0] == 'P') {
				//get
				num1 = getNum(currentGate->operand1, inputHead);
				if(currentGate->operand1[0] >= '0' && currentGate->operand1[0] <= '9'){//it is a number
					//dont get the value from a list
					num1 = operand1[0] - '0';
				}
				if(num1==-1){//it is a temp variable
					num1 = getNum(currentGate->operand1, tempHead);
				}
				//operate (do nothing)
				//set
				num1 = setNum(currentGate->operand2, num1, outputHead);
				if(num1==-1){//it is a temp variable
					setNum(currentGate->operand2, num1, tempHead);
				}
      }
      //multiplexer
      else if (currentGate->operator[0] == 'M') {
      	//operand1 has the number of selectors
      	//operand2 has the inputs
      	//operand3 has the selector variables
      	//operand4 is the output variable
        char * temp1 = malloc(30 * sizeof(char));
        char * temp2 = malloc(30 * sizeof(char));

      	//get number of selectors
        //int numSelectors = atoi(operand1);

        //walk through input variables replace everything with numbers
        char * str1 = malloc(30 * sizeof(char));
        strcpy(str1, operand2);
        char * token1;
        token1 = strtok(str1, " ");
        while(token1 != NULL) {
          if(token1[0] >= '0' && token1[0] <= '9'){//it is a number
         	 //dont get the value from a list
         	 num1 = token1[0] - '0';
          }
          else{
           num1 = getNum(token1, inputHead);//it is an input variable
           if(num1==-1){//it is a temp variable
             num1 = getNum(token1, tempHead);
           }
          }
          sprintf(temp1, "%s%d", temp1, num1);
          token1 = strtok(NULL, " ");
        }
        //printf("%s ", temp1);
        //now the input variables are stored in temp1

        //get our selector variables
        char * str2 = malloc(30 * sizeof(char));
        strcpy(str2, operand3);
        char * token2;
        token2 = strtok(str2, " ");
        while(token2 != NULL) {
          if(token2[0] >= '0' && token2[0] <= '9'){//it is a number
         	 //dont get the value from a list
         	 num2 = token2[0] - '0';
          }
          else{
           num2 = getNum(token2, inputHead);//it is an input variable
           if(num2==-1){//it is a temp variable
             num2 = getNum(token2, tempHead);
           }
          }
          sprintf(temp2, "%s%d", temp2, num2);
          token2 = strtok(NULL, " ");
        }
        //printf("%s ", temp2);
        //now the selector variables are stored in temp2

        //convert binary string to int
        //convert input variables to int
        char* start = &temp1[0];
        int decInput = 0;
        while (*start)
        {
        	decInput *= 2;
         if (*start++ == '1'){
        	 decInput += 1;
         }
        }

        //convert selector variables to int
        char* start2 = &temp2[0];
        int decSelect = 0;
        while (*start2)
        {
        	decSelect *= 2;
         if (*start2++ == '1'){
        	 decSelect += 1;
         }
        }

        //decInput contains the int representation of our inputs
        //decSelect contains the int representation of our inputs
        for(k=decSelect; k>=0; k--){
        	decInput = decInput>>1;
        }
        decInput = decInput & 1;
        //put into output variable
				num3 = setNum(currentGate->operand4, decInput, outputHead);
				if(num3==-1){//it is a temp variable
					setNum(currentGate->operand4, decInput, tempHead);
	        //printf("temp %d\n", decInput);
				}
      }
      //decoder
      else if (currentGate->operator[0] == 'D') {
      	//printf("%s %s %s %s\n", currentGate->operand1, currentGate->operand2, currentGate->operand3, currentGate->operand4);
      	//go through all inputs and convert to integer

        char * temp1 = malloc(30 * sizeof(char));
        //walk through input variables replace everything with numbers
			  char * str1 = malloc(30 * sizeof(char));
			  strcpy(str1, operand2);
			  char * token1;
			  token1 = strtok(str1, " ");
			  while(token1 != NULL) {
				  if(token1[0] >= '0' && token1[0] <= '9'){//it is a number
					  //dont get the value from a list
					  num1 = token1[0] - '0';
				  }
				  else{
				  	num1 = getNum(token1, inputHead);//it is an input variable
				  		if(num1==-1){//it is a temp variable
				  			num1 = getNum(token1, tempHead);
				  		}
				  }
				  sprintf(temp1, "%s%d", temp1, num1);
				  token1 = strtok(NULL, " ");
			  }
			 //now the input variables are stored in temp1
			  //get integer values of input variables
        char* start = &temp1[0];
        int decInput = 0;
        while (*start)
        {
        	decInput *= 2;
         if (*start++ == '1'){
        	 decInput += 1;
         }
        }
			  //use integer values to fill out output values
        char * str2 = malloc(30 * sizeof(char));
        strcpy(str2, operand3);
        char * token2;
        token2 = strtok(str2, " ");
        int count = 0;
        while(token2 != NULL) {
        	if(token2[0] >= '0' && token2[0] <= '9'){//it is a number
					 //dont get the value from a list
					 num2 = token2[0] - '0';
        	}
        	else{
        		if(count!=decInput){
							num2 = setNum(token2, 1, outputHead);//set all output variables to 1
							if(num2==-1){//it is a temp variable
								num2 = setNum(token2, 0, tempHead);
							}
        		}
        		else{
							num2 = setNum(token2, 0, outputHead);//set all output variables to 1
							if(num2==-1){//it is a temp variable
								num2 = setNum(token2, 1, tempHead);
							}
        		}
        	}
        	token2 = strtok(NULL, " ");
        	count++;
        }
      }
      else{
				//get
				num1 = getNum(currentGate->operand1, inputHead);
				if(currentGate->operand1[0] > '0' && currentGate->operand1[0] < '9'){//it is a number
					//dont get the value from a list
					num1 = operand1[0] - '0';
				}
				if(num1==-1){//it is a temp variable
					num1 = getNum(currentGate->operand1, tempHead);
				}
				num2 = getNum(currentGate->operand2, inputHead);
				if(currentGate->operand2[0] > '0' && currentGate->operand2[0] < '9'){//it is a number
					//dont get the value from a list
					num2 = operand2[0] - '0';
				}
				if(num2==-1){//it is a temp variable
					num2 = getNum(currentGate->operand2, tempHead);
				}
				//operate
				if (currentGate->operator[0] == 'A') {//and gate
					num3 = and(num1, num2);
				}
				else if (currentGate->operator[0] == 'O') {//or gate
					num3 = or(num1, num2);
				}
				else if (currentGate->operator[0] == 'N' && currentGate->operator[1] == 'A') {//nand gate
					num3 = nand(num1, num2);
				}
				else if (currentGate->operator[0] == 'N' && currentGate->operator[2] == 'R') {//nor gate
					num3 = nor(num1, num2);
				}
				else if (currentGate->operator[0] == 'X') {//xor gate
					num3 = xor(num1, num2);
				}
				//set
				num4 = setNum(currentGate->operand3, num3, outputHead);
				if(num4==-1){//it is a temp variable
					setNum(currentGate->operand3, num3, tempHead);
				}
      }
				currentGate = currentGate->next;
		}
	 	//put outputs into array
	  numst * outputPtr = outputHead; //a pointer in the input list
		for (k = m; k < (m + n); k++) {
			array[i][k] = outputPtr->value;
			outputPtr = outputPtr->next;
		}
  }
	printArray(array, numRows, m, (m + n));

	/*
	gatest * ptr = gate;
  while (ptr != NULL) {
  	printf("%s %s \n", ptr->operator, ptr->operand1);
  	ptr = ptr->next;
  }
	*/
	return 0;

}
