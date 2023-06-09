#include "Admin.h"
#include "Store.h" 
#include "Cart.h"
#include "Item.h"
using namespace std;  //STD Name-space where Library is compiled


//******************************************************************************
//                Default Constructor
//******************************************************************************

Admin::Admin(){
        //cout<<"Hit Admin Default Constructor\n";
        totalRec = 0;
        recSiz = 0;
        begnFile = 0;   
        totalRec = user.getNumRec();
        
        // Create an array of Admin objects
        usrArr = new Admin*[totalRec];

        int i;
        for(i=0; i < totalRec;i++){
            usrArr[i] = new Admin(i); // Calls constructor #2 
        }        
       
        readBin_setArray();  // Reset user array with record values read in from binary file
}

//******************************************************************************
//                    Constructor #2
//              Default Constructor calls
//******************************************************************************

Admin::Admin(int record) {
    
    //cout<<"Hit Admin Constructor #2\n";
    record = record < 0 ? 0 : record;
    user.setNumRec(record);
}


//******************************************************************************
//                        Constructor #3
//                  Called by User::readInputFile()
//******************************************************************************

Admin::Admin(string n, string e, string p) {
        //cout<<"Hit Admin Constructor #3\n";
        user.setName(n);
        user.setEmail(e);
        user.setPwrd(p);
        recSiz = 0;
        begnFile = 0;
}



//******************************************************************************
//                 Destructor
//******************************************************************************
Admin::~Admin() {    
    //cout<<"\nHit Admin Destructor().";
    delete [] usrArr; 
}


//******************************************************************************
//                 Copy Admin values to User class object
//          Reassign user2 variables based on usrArr[indx] object 
//******************************************************************************

void Admin::copy2Usr(User &user2, const int indx){
   
    cout<< "\nCopying Admin object to User object...\n";
    user2.setNumRec(usrArr[indx]->user.getNumRec());
    user2.setName(usrArr[indx]->user.getName());       
    user2.setEmail(usrArr[indx]->user.getEmail());
    user2.setPwrd(usrArr[indx]->user.getPwrd());
    user2.setCartSiz(usrArr[indx]->user.getCartSiz()); 
    int num = usrArr[indx]->user.cart.getCartIndx(0);
    user2.cart.setCartIndx(0,num);
    num = usrArr[indx]->user.cart.getCartIndx(1);
    user2.cart.setCartIndx(1,num);
    num = usrArr[indx]->user.cart.getCartIndx(2);
    user2.cart.setCartIndx(2,num);
    //usrArr[indx]->printAdUsr();
}


//******************************************************************************
//              Checks if user's login matches the binary's record
//******************************************************************************

int Admin::isUsrLogin(){    
    
    cout<<"\n\n\tUser Login\n";

    bool isName = false, isPwrd = false;
    int num = rand()%(0+totalRec);
    
    cout<<"\n\n*****Hide this after testing*****\n"<< usrArr[num]->user.getEmail()<<"\n"<< usrArr[num]->user.getPwrd()<<"\n*********************************\n";
    string tempE = usrArr[num]->user.getEmail(), tempPw =usrArr[num]->user.getPwrd(); // use for testing when you don't want to type it in each time
    
    
    cout<<"\nEmail is case sensitive. Must use lowercase letters.\n";
    do {
        cout<<"Enter email:    ";
        cin >> tempE;
    } while(!(user.isMinSize(tempE,8)) || !(user.confrmEmail(tempE)));
                  
    do{
        cout<<"Enter password: ";
        cin >> tempPw;       
    } while(!(user.isMinSize(tempPw,7)));
    cin.ignore();
       
    // look for this email in usrBin.dat. If it exists, then
    // compare email and password with record in binary file
    int indx = 0;
    if(findByEmail(tempE,indx)){
    
        isName = user.isStrEqual(usrArr[indx]->user.getEmail(),tempE);
        isPwrd = user.isStrEqual(usrArr[indx]->user.getPwrd(),tempPw); 
    
    } else { return indx; } // indx == -66. Sets flag in main()
    
    // if user inputs do NOT match record in binary then exit
    if(!isPwrd){ 
        
        cout<<"Password does not match.\n";
        return -66;   // sets flag in main()
        
    } else { return indx; }
}


/******************************************************************/ 
//          READ ADMIN BINARY FILE, RETURN 1 RECORD    
//                  & COMPARE IT TO INPUTS
/*****************************************************************/
  
void Admin::adminLogin(){
    
    cout<<"\n\tAdmin Login\n"; 
    
    string tempNam = "",
           tempPwrd = "";
    
    bool isName = false,
         isPwrd = false;
    
    readAdBin();  // Reset user array with latest records read in from binary file
    //print1Admin();
   
    //*************************************************************************
    // This section is for testing. Comment it after all testing is finished
    cout<<"\n******************************\nHide this after testing\n";
    cout<< user.getName() << endl << user.getPwrd() << endl;
    cout<<"******************************\n";
    
    // Use these when you don't want to keep typing in Admin's login info
    tempNam  = user.getName();
    tempPwrd = user.getPwrd();
    //*************************************************************************
    
    do{        
        cout<<"\nEnter username: \n";
        cin >> tempNam;
        isName = user.isMinSize(tempNam,2);
    } while(!isName);
    
    cout<<"Enter password: \n";
    cin >> tempPwrd;   
    cin.ignore();
    
    isName = user.isStrEqual(user.getName(),tempNam);
    isPwrd = user.isStrEqual(user.getPwrd(),tempPwrd);
    
    if(!isName || !isPwrd){ 
        cout<<"Username and/or password does not match.\n";       
        return;
    
    } else { adminPortal(); } // opens function that Admin is only allowed to access
}


/******************************************************************/              
//             ONLY able to access this menu  
//                  if Admin is logged in 
/*****************************************************************/

void Admin::adminPortal(){
   
    cout<<"\n\nAdmin login was successful.\n";
    int choice = 0;   
    
    do {
        cout<<"\n\n\t\tMenu\n\t"
            <<"1: Show all users\n\t"
            <<"2: Find one index\n\t"
            <<"3: Find by email\n\t"
            <<"4: Edit a User's Cart\n\t"
            <<"5: View Purchased Stats\n\t"
            <<"6: Delete a user\n\t"
            <<"7: Reset binary and text files\n\t"
            <<"8: Update Admin's Profile\n\t"
            <<"9: Logout\n\t"
            <<"Enter a number: ";
        cin>>choice;
        cin.ignore();
        
        switch(choice){
            case 1:     // Show all users
            {   
                readBin_setArray();  // Reset user array with records read in from latest binary file
                printAllUsr(); // Print usrArr[]   
                pause();
                break;
            } 
            case 2:     // Find one index
            { 
                cout<<"\nWhich record are you looking for?\n";
                int indx = 0;
                getIndex(indx);    // Find user by index in binary file    
                pause();
                break;
            } 
            
            case 3:     // Find by email
            {   
                getByEmail();  // Find user by email in binary file 
                pause();
                break;
            }      
            case 4:       // Edit Cart
            {    
                editCart();                
                pause();
                break;
            }
            case 5:                   // View Purchased Stats
            {                    
                readBin_setArray();  // Reset user array with records read in from latest binary file
                printItemStats();    // Print store results
                pause();
                break;
            }
            case 6:      // Delete a user's record in binary & text files
            {   
                deleteUsr();                 
                pause();
                break;
            }             
            case 7:   // Reset files by erasing binary & text file, then creates records in 
            {        // User binary with records. Use after testing & altering records.      
                user.readInputFile();
                readBin_setArray();
                break;
            }
            case 8:     // Update Admin's profile
            {                
                updateAdmin();        
                break;
            } 
            default: {
                cout<<"\nLogged Out.\n";                
                return;
            }
        }
    } while(choice>=1 && choice<=8);
}


//******************************************************************
//                     Read binary file & reset usrArr[]
//******************************************************************
void Admin::readBin_setArray(){
    
    cout<<"\nReading User binary file and resetting usrArr...\n";
    
    ifstream inBin;
    string file = "usrData.dat";
    inBin.open(file.c_str(), ios::in | ios::binary); 
    if(!inBin.is_open()){ cout<<"\nError opening "<<file<<endl; return;}
        
    long cursor = 0L,
         thisSum = 0L;
    int  i = 0;
    int num = 0;
    
    // Accumulate the size of each record and the beginning bit location for each record
    while(i < totalRec){ 
           
        thisSum = 0L;
        //cout<<"\n\n i=" << i <<" cursor=" << cursor << " thisSum=" << thisSum <<endl;
        
        
        // set is set to the beginning of the cursor's value
        inBin.seekg(cursor,ios::beg);  
        
       
        // Read & set numRec in User object
        // Accumulate number of bits for numRec
        inBin.read(reinterpret_cast<char *>(&num) , sizeof(int));   
        usrArr[i]->user.setNumRec(num); 
        thisSum += sizeof(int);  
        //cout<<"thisSum=="<<thisSum<<endl;
        
        
        
        // Read & set namSiz in User object
        // Resize, read & set name in User object
        // Accumulate number of bits for namSize + name
        unsigned short size;
        inBin.read(reinterpret_cast<char *>(&size), sizeof(unsigned short) ); 
        usrArr[i]->user.setNamSiz(size); 
        
        string tempStr = "";
        tempStr.resize(usrArr[i]->user.getNamSiz());   
        inBin.read( &tempStr[0] , size*sizeof(char) );      
        usrArr[i]->user.setName(tempStr);             
               
        thisSum += ( sizeof(size) + (size*sizeof(char)) ); 
        //cout<<"thisSum=="<<thisSum<<endl;    
         
        
      
        // Read & set emaiSiz in User object
        // Resize, read & set email in User object
        // Accumulate number of bits for emaiSiz + email
        inBin.read(reinterpret_cast<char *>(&size), sizeof(unsigned short) );             
        usrArr[i]->user.setEmaiSiz(size);   
        
        tempStr.resize(usrArr[i]->user.getEmaiSiz());
        inBin.read(&tempStr[0], size*sizeof(char)); 
        usrArr[i]->user.setEmail(tempStr);          
        
        thisSum += ( sizeof(size) + size*sizeof(char) );
        //cout<<"thisSum=="<<thisSum<<endl;        
        
        
        
        // Read & set pwrdSiz in User object
        // Resize, read & set password in User object
        // Accumulate number of bits for pwrdSiz + password
        inBin.read(reinterpret_cast<char *>(&size), sizeof(unsigned short));           
        usrArr[i]->user.setPwrdSiz(size);
       
        tempStr.resize(size);
        inBin.read(&tempStr[0], size*sizeof(char) );    
        usrArr[i]->user.setPwrd(tempStr);   
        
        thisSum += ( sizeof(size) + (size*sizeof(char)) );
        //cout<<"thisSum=="<<thisSum<<endl;                
        
        
        
        // Read & set cartSiz in class object
        // Accumulate number of bits for cartSiz
        inBin.read(reinterpret_cast<char *>(&num) , sizeof(int)); 
        usrArr[i]->user.setCartSiz(num); 
        thisSum += sizeof(num);  
        //cout<<"thisSum=="<<thisSum<<endl;
        
        
        
        // Read each element in Cart array
        // Accumulate number of bits in Cart array    
        inBin.read(reinterpret_cast<char *>(&num) , sizeof(int));
        usrArr[i]->user.cart.setCartIndx(0,num);
        thisSum += sizeof(num);  
        
        inBin.read(reinterpret_cast<char *>(&num) , sizeof(int));
        usrArr[i]->user.cart.setCartIndx(1,num);
        thisSum += sizeof(num);  
        
        inBin.read(reinterpret_cast<char *>(&num) , sizeof(int));
        usrArr[i]->user.cart.setCartIndx(2,num);
        thisSum += sizeof(num);  
   
        
        // Reset cursor to read the next record 
        cursor  += thisSum;            
        //cout << "\n thisSum = " << thisSum << " cursor = " << cursor << endl;
        
        
        // if its the first record,then set begnGile to zero
        long bFile = (i==0)? 0L : (cursor-thisSum); 
        usrArr[i]->setBegnFile(bFile);
        usrArr[i]->setRecSiz(thisSum);
       
        //usrArr[i]->getCartArr(); // Prints user's cart[]
        //usrArr[i]->printAdUsr();
        i++; 
    }    
    
    i = (i > totalRec) ? i : totalRec;
    setItemStats();    
    inBin.close();              
}



/******************************************************************/ 
//                  
/******************************************************************/ 
void Admin::setItemStats(){
    
    //cout << "\n\tHit setItemStats().\n";  
    
     int totalPurchd = 0, item0Purchd = 0, item1Purchd = 0, item2Purchd = 0;
     
    // Loop through every record in usrArr[]  
    for(int i=0; i < totalRec; i++){      
     
        //usrArr[i]->getCartArr();   
        
        float sum = 0;
        int sumItms = 0;
         
        // Loop through number of items in store
        for(int cartIndx=0; cartIndx < NUMITEMS; cartIndx++){           
       
                        
            // set this person's cart for item 0, 1, 2
            int cartVal = usrArr[i]->user.cart.getCartIndx(cartIndx); //  cart[0] = item 0          
                                                                     //  cart[1] = item 1  
                                                                     //  cart[2] = item 2                
           
            if( cartIndx == 0 && cartVal > 0){ // cart[0] > 0 ? itemStats.vote[0] = +1                   

                item0Purchd  += cartVal;
                //cout << " Hit cartIndx == 0  "<< "item0Purchd = " << item0Purchd << "  cartVal = " << cartVal << endl;
                sumItms += cartVal;
                sum += (cartVal* (store.item[cartIndx]->getPrice() ));
                
            } else if(cartIndx == 1 && cartVal > 0){  //  vote[count] = 2 ? itemStats.vote[1] = +1

                item1Purchd  += cartVal;
                //cout << " Hit cartIndx == 1  " << "item1Purchd = " << item1Purchd << "  cartVal = " << cartVal << endl;
                sumItms += cartVal;
                sum += (cartVal* (store.item[cartIndx]->getPrice() ));
                 
            } else if(cartIndx == 2 && cartVal > 0){   //  vote[count] = 3 ? itemStats.vote[2] = +1
                
                item2Purchd  += cartVal;
                //cout << " Hit cartIndx == 2  " << "item2Purchd = " << item2Purchd << "  cartVal = " << cartVal << endl;
                sumItms += cartVal;
                sum += (cartVal* (store.item[cartIndx]->getPrice() ));
            }
            else {
                cout<<"";
            }
            
            // Reset this user's cart variables           
            usrArr[i]->user.cart.setTotItm(sumItms);
            usrArr[i]->user.cart.setCartTotal(sum);
            
        } // ends cartIndx < NUMITEMS        
        
    } // ends i < totalRec 
     
    // the how many times each time has been purchased
    itemStats.setCartIndx(0, item0Purchd); 
    itemStats.setCartIndx(1, item1Purchd); 
    itemStats.setCartIndx(2, item2Purchd); 
    
    totalPurchd = item0Purchd + item1Purchd + item2Purchd;
    itemStats.setTotItm(totalPurchd);
}



/******************************************************************/ 
//
/******************************************************************/ 
void Admin::printItemStats(){
    
    cout<<"\n\t\t\t*****Statistics for Purchased Items*****";
    
    // Loop through usrArr[] & print their order cart[] 
    cout << endl << endl;
    for(int i=0; i < totalRec; i++){  
        cout << "\t";
        usrArr[i]->getCartArr();    
    }
    
    cout << fixed << setprecision(0) << endl << endl << endl;     
     
    float avg = 0.0f;
    
    for(int i=0; i < NUMITEMS; i++){ 
        
        cout << "\t  Item " << i+1 << setw(20) << store.item[i]->getItem(0);
             
        float totalPurchd = itemStats.getTotItm();
        avg = 100*(itemStats.getCartIndx(i)/totalPurchd);
        
        cout << " purchased: "<< itemStats.getCartIndx(i) << " times. "<< setw(3) << avg << "%  ";
        getChart(itemStats.getCartIndx(i));
    }    
    cout << "\n\t  Total items ever purchased:  " << itemStats.getTotItm() << endl;
}


//*********************************************************
//           Aggregating instance of Item Class
//*********************************************************  
void Admin::getCheckout(int indx){
    
    float sum = 0.0f;
    int sumItms = 0;
    
    cout << "\n\n\t***CHECKOUT***\n\n";
   

    for(int i=0; i < NUMITEMS; i++){

        int quantity = usrArr[indx]->user.cart.getCartIndx(i);

        if(quantity > 0){
            
            sum += (quantity*store.item[i]->getPrice());            
            
            cout << setw(19) << store.item[i]->getItem(0);
            cout << setw(3) << "$" << store.item[i]->getPrice() 
                 << " x " << quantity 
                 << " = " << sum << endl;
        }
    }        
    cout << "-----------------------------------------\n";
    cout << setw(34) << "Total $" << usrArr[indx]->user.cart.getCartTotal() << endl;
    cout << setw(34) << "Total Items: " << usrArr[indx]->user.cart.getTotItm() <<endl<<endl;     
}



/******************************************************************/ 
//
/******************************************************************/ 
void Admin::getChart(int qSum) const{   
    for(int i = 0; i < qSum; i++){        
        cout<<"*";  
    }
    cout<<endl;
}


/******************************************************************/ 
//
/******************************************************************/ 
void Admin::getCartArr(){
    cout << "\n\t\t\t\t Record " << setw(2) << this->user.getNumRec();  
    cout<<"  cart[ ";
    this->user.cart.prntCart(); 
}



/******************************************************************/ 
//                     Reset cartSiz in User object
//                   and rewrite binary & text files
/******************************************************************/ 
void Admin::editCart(){ 
    
    int num1 = 1, 
        num2 = 1,
        num3 = 1,    
        indx = 0;  
    
    cout<<"\nWhich record do you want to edit?\n";
    getIndex(indx);          
    
    do {
        cout<<"\n\nEnter sushi quantity for item 1. Number 1-10\n";
        cin >> num1;   
    } while(!(num1>=0 && num1<=10));
    
    do {
        cout<<"\nEnter sushi quantity for item 2. Number 1-10\n";
        cin >> num2;   
    } while(!(num2>=0 && num2<=10));
    
    do {
        cout<<"\nEnter sushi quantity for item 3. Number 1-10\n";
        cin >> num3;   
    } while(!(num3>=0 && num3<=10));
    cin.ignore();
    
    usrArr[indx]->user.cart.setCartArr(num1,num2,num3);     
    
    long recLoc = usrArr[indx]->begnFile;
    usrArr[indx]->user.reWrtBin(recLoc);
    readBin_setArray(); // Reset usrArr after the binary file is updated   
    cout<<"\n\nRecord successfully updated.\n";
    usrArr[indx]->printAdUsr();
    printItemStats();     // Print store results
}


/******************************************************************/ 
//             Delete 1 record in User binary file
/******************************************************************/ 

void Admin::deleteUsr(){

    int i, indx = 0;   
    
    cout<<"\nWhich record do you want to delete?\n";
    getIndex(indx); 
                
 
    string temp = "";
    for(i=0; i < (usrArr[indx]->user.getNamSiz()) ;i++){
        temp += "x";
    }
    usrArr[indx]->user.setName(temp);
    
    temp = "";
    for(i=0; i < (usrArr[indx]->user.getEmaiSiz()); i++){
        temp += "x";
    }
    usrArr[indx]->user.setEmail(temp);
    
    temp = "";
    for(i=0; i < (usrArr[indx]->user.getPwrdSiz()); i++){
        temp += "x";
    }
    usrArr[indx]->user.setPwrd(temp);       // Reset password 
    usrArr[indx]->user.setCartSiz(0);       // Reset cartSiz
    usrArr[indx]->user.reWrtBin(usrArr[indx]->begnFile); // rewrites this record in binary & text files  
    readBin_setArray();          // Reset usrArr[] after the binary file is updated  
    cout<<"\n\nRecord successfully deleted.";
    usrArr[indx]->printAdUsr();          // confirm this record was reset
}


/*****************************************************************/
//           if email is found, then return class obj with record  
//              else if email is NOT found return false
//                     isUsrLogin() calls it.
/*****************************************************************/

bool Admin::findByEmail(string tempEmail, int &indx){
    
    cout <<"\n\nLooking for " << tempEmail << "...\n\n";   
    
    bool foundEmail = false;
    int count = 0;         // record counter      
    string binaryEmail = "";  // Read email & save to a temp string

    
    while( (!foundEmail) && (count < totalRec) ){
        
        int size = usrArr[count]->user.getEmaiSiz();
        binaryEmail.resize(size);
        binaryEmail = usrArr[count]->user.getEmail();
        foundEmail = user.isStrEqual(tempEmail, binaryEmail); // looking for email inputted by user
        //cout<< "\n\t" << tempEmail << "==" << binaryEmail << endl ;
        
        if(foundEmail){
            indx = usrArr[count]->user.getNumRec();
            return true;
        } 
        else { count++; }
       
    }    
      
    indx = -66; // used as a flag
    cout<<"\nUnable to locate email.\n";
    return false; 

}
    
/*****************************************************************/
//                   Find email & print
//                  adminNewClassal() calls it
/*****************************************************************/
void Admin::getByEmail(){
    
       
    int ind = 0, count = 0, size = 0;
    int num = rand()%(0+totalRec);
    string tempEmail = "";
    
    
    cout<<"\n\n*****Hide this after testing*****\n"<< usrArr[num]->user.getEmail()<<"\n*********************************\n";
    tempEmail = usrArr[num]->user.getEmail(); // use for testing when you don't want to type it in each time
    //tempEmail = "homer@simp.com";
    //tempEmail = "marge@simp.com";
    //tempEmail = "lucy@beatles.com";  
    
    
    cout<<"\nEnter the email you want to find: ";
    cin >> tempEmail;
    cin.ignore();    
            
    cout <<"\n\nLooking for " << tempEmail << "...\n\n";   
    
    bool foundEmail = false;  
    string binaryEmail = "";  // Read email & save to a temp string

    
    while( (!foundEmail) && (count < totalRec) ){
        
        //size = usrArr[count]->user.getEmaiSiz(); // get emaiSiz read from binary
        //binaryEmail.resize(size);                 // resize string to emaiSiz
        binaryEmail = usrArr[count]->user.getEmail(); // get email read from binary
        foundEmail = user.isStrEqual(tempEmail, binaryEmail); // compare binary email with email inputted by user
        //cout<< "\n\t" << tempEmail << "==" << binaryEmail << endl ;
        
        if(foundEmail){
            int indx = usrArr[count]->user.getNumRec();
            cout<<"Located " << tempEmail << "'s profile.\n";
            usrArr[indx]->printAdUsr();
            count = totalRec;
        } 
        else {  count++; }       
    }        
   
    if(!foundEmail){ cout<<"\nUnable to locate email.\n"; } 
}



/*****************************************************************/
//               Validate user's input  
//               and print this record
/*****************************************************************/

void Admin::getIndex(int &indx){
   
    do {
        cout<<"Enter a number between 0 and " << (totalRec-1) <<endl;
        cin>>indx;
    } while (!(indx>=0 && indx <= (totalRec-1)));
    
    cout << "\nFound record " << indx << "\n";
    usrArr[indx]->printAdUsr(); 
}


/*****************************************************************/
//            Read Admin's binary file, find 1st record 
//             & return 1 record as a Admin class object
/*****************************************************************/

void Admin::readAdBin(){ 
     
    cout<<"\nReading Admin Binary...\n";
    
    ifstream inBin;
    string file = "admin.dat";    
    inBin.open(file.c_str(), ios::in | ios::binary); 
    if(!inBin.is_open()){ cout<<"\nError opening "<<file<<endl; return;}
        
    long cursor = 0L;
     
    // Set cursor at the very beginning of Admin's binary file              
    inBin.seekg(cursor,ios::beg); 

    // Read the value of namSiz & set Admin member variable
    unsigned short size;
    inBin.read(reinterpret_cast<char *>(&size), sizeof(unsigned short) );
    user.setNamSiz(size);
    
    // Read name & set Admin member variable
    string str;
    str.resize(size);
    inBin.read(&str[0] , size*sizeof(char) );
    user.setName(str);
    
    
    // Read the value of emaiSiz and set Admin object        
    inBin.read(reinterpret_cast<char *>(&size), sizeof(unsigned short) );      
    user.setEmaiSiz(size);
     
    // Read email & set Admin member variable
    str="";
    str.resize(size);
    inBin.read(&str[0], size*sizeof(char) );
    user.setEmail(str);

    
    // Read the value of pwrdSiz and set Admin object     
    inBin.read(reinterpret_cast<char *>(&size), sizeof(unsigned short));  
    user.setPwrdSiz(size);
    
    // Read name & set Admin member variable
    str = "";
    str.resize(size);    
    inBin.read(&str[0], size*sizeof(char) );        
    user.setPwrd(str);  
  
    inBin.close(); // close file
}


/******************************************************************/ 
//                  Write 1 Admin record to text file  
/*****************************************************************/

void Admin::wrtAdTxt(){
    
    ofstream outTxt; 
    outTxt.open("admin.txt",ios::out); // DOES NOT appends content to the current content of the file.
    if(!outTxt.is_open()){ cout<<"\nError opening admin.txt\n"; exit(0);}
    
    outTxt<<"namSiz:  "<<user.getNamSiz()<<endl; // write the size of this string to text file
    outTxt<<"name:    "<<user.getName()<<endl;   // write this string to text file
    outTxt<<"emaiSiz: "<<user.getEmaiSiz()<<endl;// write the size of this string to text file
    outTxt<<"email:   "<<user.getEmail()<<endl;  // write this string to text file
    outTxt<<"pwrdSiz: "<<user.getPwrdSiz()<<endl; // write the size of this string to text file
    outTxt<<"pwrd:    "<<user.getPwrd()<<endl;// write this string to text file
    outTxt.close(); // close file
}


/******************************************************************/  
//                   Write 1 Admin record to binary file   
/*****************************************************************/

void Admin::wrtAdBin(){
        
    ofstream outBin;
    outBin.open("admin.dat",ios::out | ios::binary); // DOES NOT appends content to the current content of the file.
    if(!outBin.is_open()){ cout<<"\nError opening admin.dat\n";}      
    
    // Write name's size & name to binary file
    unsigned short size = user.getNamSiz();
    outBin.write(reinterpret_cast<char *>(&size), sizeof(unsigned short)); 
    outBin.write(user.getName().c_str(), size*sizeof(char)); 
        
    // Write email's size & email to binary file
    size = user.getEmaiSiz();
    outBin.write(reinterpret_cast<char *>(&size), sizeof(unsigned short));      
    outBin.write( user.getEmail().c_str(), size*sizeof(char)); 
        
    // Write password's size & password to binary file
    size = user.getPwrdSiz();
    outBin.write(reinterpret_cast<char *>(&size), sizeof(unsigned short)); 
    outBin.write(user.getPwrd().c_str(), size*sizeof(char));         
      
    outBin.close(); // close file    
}


/******************************************************************/ 
//               Calls User functions to rewrite Admin's 
//                  record in binary & text 
/******************************************************************/ 
void Admin::updateAdmin(){ 
    
    if(user.isName() && user.isEmail() && user.isPwrd()){
        print1Admin();
        wrtAdTxt();
        wrtAdBin(); 
     
    } else { cout<<"\n\nUnable to update Admin's profile.\n";}
} 

/*****************************************************************/
//      PRINT array of all User's that were read from binary  
/*****************************************************************/
void Admin::printAllUsr() const {
    for(int i=0; i < totalRec;i++){
        usrArr[i]->printAdUsr();
    }  
}

/*****************************************************************/
//                     PRINT 1 Admin'S INFO  
/*****************************************************************/

void Admin::print1Admin()const{  
    cout<<endl<<endl        
        <<"NamSiz:    "<< user.getNamSiz() <<endl
        <<"Name:      "<< user.getName()   <<endl
        <<"EmaiSiz:   "<< user.getEmaiSiz() <<endl
        <<"Email:     "<< user.getEmail()   <<endl
        <<"PwrdSiz:   "<< user.getPwrdSiz() <<endl
        <<"Password:  "<< user.getPwrd() <<endl;    
}


/*****************************************************************/
//               PRINT 1 User + Admin member variables
/*****************************************************************/

void Admin::printAdUsr() const{
    
    cout<<"\n\tADMIN'S VIEW";
    user.printUsrRec();
    cout<<"Record Size    " << recSiz   << endl
        <<"Start of file: " << begnFile << endl;
}


/*****************************************************************/
//               PRINT 1 User + Admin member variables
/*****************************************************************/

void Admin::printAdUsr(int indx) const{
    
    cout<<"\n\tADMIN'S VIEW";
    usrArr[indx]->user.printUsrRec();
    cout<<"Record Size    " << usrArr[indx]->recSiz   <<endl
        <<"Start of file: " << usrArr[indx]->begnFile <<endl;
}


//******************************************
//      pause screen before continuing
//******************************************
void Admin::pause(char ch) {
    
    string msg = ch=='r' ? "roll" : "continue";    
    cout<<endl<<setw(6)<<' '<<"Press enter to " << msg << ".";   
    cin.get();
}