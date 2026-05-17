#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
void setColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((bgColor << 4) | textColor));
}
typedef struct
{
    int roomNumber;
    char status[20];
    char category[50];
    int price;
} room;
typedef struct
{
    char day[3];
    char month[3];
    char year[5];

} date;
typedef struct
{
    int reservationID;
    int roomNumber;
    char status[20];
    char customerName[100];
    char nationalID[80];
    int nights;
    date cid;
    char email[1000];
    char mobileNumber[15];
} Reservation;
typedef struct
{
    char name[150];
    char pass[50];
} login;
int CLRM()
{
    int count =0;
    FILE *f1=fopen("rooms.txt","r");
    if(f1==NULL)
    {
        printf("Error");
        exit(1);
    }
    while(!feof(f1))
    {

        char x[500];
        while(fgets(x,500,f1))
        {
            x[strcspn(x,"\n")]=0;
            count++;
        }

    }
    fclose(f1);
    return count;
}

void userID()
{
    char ch;
    int z,y,i=0;
    login x,p;
    setColor(5,0);
    printf("Enter username:");
    setColor(7,0);
    fgets(x.name,150,stdin);
    x.name[strcspn(x.name,"\n")]=0;
    setColor(5,0);
    printf("Enter password:");
    setColor(7,0);
    while((ch=getch())!=13)
    {
        if(ch==8)
        {
            if(i>0)
            {
                printf("\b \b");
                i--;
            }

        }
        else
        {
            x.pass[i++]=ch;
            printf("*");
        }
    }
    x.pass[i]='\0';
    x.pass[strcspn(x.pass,"\n")]=0;
    FILE *f1=fopen("users.txt","r");
    if(f1==NULL)
    {
        printf("Error");
        exit(1);
    }
    while(!feof(f1))
    {
        fscanf(f1,"%s",&p.name);
        fscanf(f1,"%s",&p.pass);
        z=strcmp(x.name,p.name);
        y=strcmp(x.pass,p.pass);
        if(z==0 && y==0)
            break;
    }
    if (z==0&&y==0)
    {
        setColor(10,0);
        printf("\nLogin successful\n");
        setColor(7,0);
    }
    else
    {
        setColor(12,0);
        printf("\nInvalid username or password\n");
        setColor(7,0);
        printf("Please re-enter the username and password\n");
        userID();
    }
    fclose(f1);

}
void mainMenu(int n,Reservation* r,room* z,int c)
{
    int x;
    char p[100];
    printf("__________________________\n");
    setColor(1,0);
    printf("\n==={HOTEL MAIN MENU}===\n");
    setColor(14,0);
    printf("1. Reserve a Room\n2. Check-in\n3. Cancel Reservation\n4. Check-out\n5. Check Room Availability\n6. View Customer Details\n7. Edit Reservation Details\n8. Query\n9. Reservation report\n");
    setColor(12,0);
    printf("0. Exit\n");
    setColor(7,0);
    printf("__________________________\n");
    printf("Please select an option:");
    fgets(p, sizeof(p), stdin);
    p[strcspn(p, "\n")] = 0;
    while(p[0] < '0' || p[0] > '9' || p[1]!='\0')
    {
        setColor(12,0);
        printf("Incorrect value, please enter another option:");
        setColor(7,0);
        fgets(p, sizeof(p), stdin);
        p[strcspn(p, "\n")] = 0;
    }
    x= atoi(p);
    switch(x)
    {
    case 1:
    {
        RR(z,n,r,&c);
        sort(r,c);
        break;
    }
    case 2:
    {
        checkIn(r,z,n,c);
        break;
    }
    case 3:
    {
        cancelReservation(&c,r,n,z);
        break;
    }
    case 4:
    {
        checkout(&c,r,n,z);
        break;
    }
    case 5:
    {
        TRA(n,z);
        break;
    }
    case 6:
    {
        VCD(c,r);
        break;
    }
    case 7:
    {
        editReservation(r,c,z,n);
        sort(r,c);
        break;
    }
    case 8:
    {
        Query(n,r,z);
        break;
    }
    case 9:
    {
        report(r,c);
        break;
    }
    case 0:
    {
        exit(0);
    }
    }
    Exit(n,r,z,c);

}

int Load(Reservation *r,int n)
{
    int count=0;
    int i;
    char x[500];
    FILE *f1=fopen("Reservations.txt","r");
    if(f1==NULL)
    {
        printf("Error");
        exit(1);
    }

    for(i=0; i<n; i++)
    {
        if(fgets(x,500,f1))
        {
            char *token=strtok(x,",");
            r[i].reservationID=token?atoi(token):0;
            token=strtok(NULL,",");
            r[i].roomNumber=token?atoi(token):0;
            token=strtok(NULL,",");
            strcpy(r[i].status,token?token:"");
            token=strtok(NULL,",");
            strcpy(r[i].customerName,token?token:"");
            token=strtok(NULL,",");
            strcpy(r[i].nationalID,token?token:"");
            token=strtok(NULL,",");
            r[i].nights=token?atoi(token):0;
            token=strtok(NULL,",");
            if(token)
            {
                sscanf(token,"%2s-%2s-%4s",&r[i].cid.day,&r[i].cid.month,&r[i].cid.year);
            }
            token=strtok(NULL,",");
            strcpy(r[i].email,token);
            token=strtok(NULL,",\n");
            strcpy(r[i].mobileNumber,token?token:"");
            count++;
        }

    }


    fclose(f1);
    return count;
}
void VCD(int c,Reservation *r)
{
    int i,id,rm,done=0;
    char a[500];

    printf("Enter 1 for reservationID or Enter 0 for Room number:");;
    scanf("%s",a);

    if(strcmp(a,"1")==0)
    {
        printf("Enter reservation ID:");
        scanf("%d",&id);
        for (i=0; i<c; i++)
        {
            if(id==r[i].reservationID)
            {
                printf("The details are:\nCustomer Name:%s\nNational ID:%s\nEmail:%s\nMobile Number:%s"

                       ,r[i].customerName,r[i].nationalID, r[i].email,r[i].mobileNumber);
                done=1;
            }
        }
    }
    else if(strcmp(a,"0")==0)
    {
        printf("Enter room number:");
        scanf("%d",&rm);
        for (i=0; i<c; i++)
        {
            if(rm==r[i].roomNumber)
            {
                printf("The details are:\nCustomer Name:%s\nNational ID:%s\nEmail:%s\nMobile Number:%s"
                       ,r[i].customerName,r[i].nationalID, r[i].email,r[i].mobileNumber);
                done=1;
            }
        }

    }
    else
    {
        setColor(12,0);
        printf("Error\n");
        setColor(7,0);
        VCD(c,r);
    }


    if(done==0)
    {
        printf("Customer details not found");
        while (getchar() != '\n');

    }
}

void  loadrooms(room *z,int n2)
{
    int i;
    char x[500];
    FILE *f1=fopen("rooms.txt","r");
    if(f1==NULL)
    {
        printf("Error");
        exit(1);
    }
    for(i=0; i<n2; i++)
    {
        if(fgets(x,500,f1))
        {
            char *token=strtok(x," ");
            z[i].roomNumber=token?atoi(token):0;
            token=strtok(NULL," ");
            strcpy(z[i].status,token?token:"");
            token=strtok(NULL," ");
            strcpy(z[i].category,token?token:"");
            token=strtok(NULL," ");
            z[i].price=token?atoi(token):0;
        }
    }

    fclose (f1);

}
void RR(room *z, int n, Reservation *t, int *c)
{
    Reservation r;
    srand(time(0));

    int i, min = 100000, max = 999999, flag = 0;
    int reservationId = min + rand() % (max - min + 1);

    while (!flag)
    {
        flag = 1;
        for (i = 0; i <= *c; i++)
        {
            if (r.reservationID == reservationId)
            {
                reservationId = min + rand() % (max - min + 1);
                flag = 0;
                break;
            }
        }
    }

    r.reservationID = reservationId;

    printf("The new reservation ID is: %d\n", r.reservationID);


    printf("Enter the Customer name: ");

    fgets(r.customerName, sizeof(r.customerName), stdin);
    r.customerName[strcspn(r.customerName, "\n")] = 0;
    for (i = 0; r.customerName[i] != '\0'; i++)
    {
        if (!((r.customerName[i] >= 'A' && r.customerName[i] <= 'Z') ||
                (r.customerName[i] >= 'a' && r.customerName[i] <= 'z') ||
                r.customerName[i] == ' '))
        {
            setColor(12,0);
            printf("Invalid name\n");
            setColor(7,0);
            printf("Re-enter: \n");
            fgets(r.customerName, sizeof(r.customerName), stdin);
            r.customerName[strcspn(r.customerName, "\n")] = 0;
            i = -1;
        }
    }


    printf("Enter the Customer national ID: ");
    fgets(r.nationalID, sizeof(r.nationalID), stdin);
    r.nationalID[strcspn(r.nationalID, "\n")] = 0;
    for (i = 0; r.nationalID[i] != '\0'; i++)
    {
        if (!(r.nationalID[i] >= '0' && r.nationalID[i] <= '9'))
        {
            setColor(12,0);
            printf("Invalid national ID, re-enter: \n");
            setColor(7,0);
            fgets(r.nationalID, sizeof(r.nationalID), stdin);
            r.nationalID[strcspn(r.nationalID, "\n")] = 0;
            i = -1;
        }
    }


    printf("Enter the email: ");
    fgets(r.email, sizeof(r.email), stdin);
    r.email[strcspn(r.email, "\n")] = 0;
    while (strchr(r.email, '@') == NULL || strchr(r.email, '.') == NULL)
    {
        setColor(12,0);
        printf("Invalid email, re-enter: \n");
        setColor(7,0);
        fgets(r.email, sizeof(r.email), stdin);
        r.email[strcspn(r.email, "\n")] = 0;
    }


    printf("Enter the mobile number: ");
    while (1)
    {
        fgets(r.mobileNumber, sizeof(r.mobileNumber), stdin);
        r.mobileNumber[strcspn(r.mobileNumber, "\n")] = 0;

        if (strlen(r.mobileNumber) == 11 && r.mobileNumber[0] == '0' &&
                r.mobileNumber[1] == '1')
        {
            int valid = 1;
            for (i = 0; i < 11; i++)
            {
                if (r.mobileNumber[i] < '0' || r.mobileNumber[i] > '9')
                {
                    valid = 0;
                    break;
                }
            }
            if (valid) break;
        }
        setColor(12,0);
        printf("Invalid mobile number, re-enter: \n");
        setColor(7,0);
    }


    printf("Enter the check-in month (in MM format): ");
    while (1)
    {
        scanf("%s", r.cid.month);
        if (strcmp(r.cid.month, "01") == 0 || strcmp(r.cid.month, "02") == 0 ||
                strcmp(r.cid.month, "03") == 0 || strcmp(r.cid.month, "04") == 0 ||
                strcmp(r.cid.month, "05") == 0 || strcmp(r.cid.month, "06") == 0 ||
                strcmp(r.cid.month, "07") == 0 || strcmp(r.cid.month, "08") == 0 ||
                strcmp(r.cid.month, "09") == 0 || strcmp(r.cid.month, "10") == 0 ||
                strcmp(r.cid.month, "11") == 0 || strcmp(r.cid.month, "12") == 0)
        {
            break;
        }
        else
        {
            setColor(12,0);
            printf("Invalid month, re-enter (in MM format): ");
            setColor(7,0);
        }
    }

    printf("Enter the check-in day (in DD format): ");
    while (1)
    {
        scanf("%s", r.cid.day);
        int day = atoi(r.cid.day);
        int month = atoi(r.cid.month);
        if (strlen(r.cid.day) == 2 && isdigit(r.cid.day[0]) && isdigit(r.cid.day[1]))
        {
            if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
                    month == 10 || month == 12) && (day >= 1 && day <= 31))
            {
                break;
            }
            else if ((month == 4 || month == 6 || month == 9 || month == 11) &&
                     (day >= 1 && day <= 30))
            {
                break;
            }
            else if (month == 2 && (day >= 1 && day <= 28))
            {
                break;
            }
        }
        setColor(12,0);
        printf("Invalid day for given month, re-enter (in DD format): ");
        setColor(7,0);
    }

    printf("Enter the check-in year (in YYYY format): ");
    while (1)
    {
        scanf("%s", r.cid.year);
        if (strlen(r.cid.year) == 4 && isdigit(r.cid.year[0]) && isdigit(r.cid.year[1]) &&
                isdigit(r.cid.year[2]) && isdigit(r.cid.year[3]))
        {
            break;
        }
        else
        {
            setColor(12,0);
            printf("Invalid year, re-enter (in YYYY format): ");
            setColor(7,0);
        }
    }
    char p[20];

    printf("Enter the number of nights: ");
    while (getchar() != '\n');
    fgets(p, sizeof(p), stdin);
    p[strcspn(p, "\n")] = 0;
    for (i = 0; p[i] != '\0'; i++)
    {
        if (!(p[i] >= '0' && p[i] <= '9'))



        {
            setColor(12,0);
            printf("Invalid num, re-enter: \n");
            setColor(7,0);
            fgets(p, sizeof(p), stdin);
            p[strcspn(p, "\n")] = 0;
            i = -1;
        }
    }
    r.nights= atoi(p);
    char f[500];
    int flag2 = 0;
    while (getchar() != '\n');

    do
    {
        printf("Enter the category: ");
        fgets(f, sizeof(f), stdin);
        f[strcspn(f, "\n")] = 0;
        for (i = 0; i < n; i++)
        {
            if (strcasecmp(f, z[i].category) == 0 && strcasecmp(z[i].status, "Available") == 0)
            {
                r.roomNumber = z[i].roomNumber;
                strcpy(z[i].status, "Reserved");
                strcpy(r.status, "unconfirmed");
                flag2 = 1;
                break;
            }
        }
        if (!flag2)
        {
            setColor(12,0);
            printf("Category unavailable, please re-enter.\n");
            setColor(7,0);
        }
    }
    while (!flag2);
    char a[100];
    printf("enter 1 to save Press any key to continue to unsave and continue:" );
    scanf("%s",a);
    if(strcmp(a,"1")==0)
    {
        (*c)++;
        t[*c-1].reservationID=r.reservationID;
        t[*c-1].roomNumber=r.roomNumber;
        strcpy(t[*c-1].status,r.status);
        strcpy(t[*c-1].customerName,r.customerName);
        strcpy(t[*c-1].nationalID,r.nationalID);
        t[*c-1].nights=r.nights;
        strcpy(t[*c-1].cid.day,r.cid.day);
        strcpy(t[*c-1].cid.month,r.cid.month);
        strcpy(t[*c-1].cid.year,r.cid.year);
        strcpy(t[*c-1].email,r.email);
        strcpy(t[*c-1].mobileNumber,r.mobileNumber);
        FILE *f1 = fopen("rooms.txt", "w");
        FILE *f2 = fopen("Reservations.txt", "w");
        for (i = 0; i < n; i++)
        {
            fprintf(f1, "%d %s %s %d\n", z[i].roomNumber, z[i].status, z[i].category, z[i].price);
        }
        for(i=0; i<*c; i++)
        {
            fprintf(f2,"%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n", t[i].reservationID, t[i].roomNumber, t[i].status,
                    t[i].customerName, t[i].nationalID, t[i].nights,
                    t[i].cid.day, t[i].cid.month, t[i].cid.year,
                    t[i].email, t[i].mobileNumber);
        }
        fclose(f1);
        fclose(f2);
    }
}
void TRA(int n,room* z)
{
    char x[]="Available";
    printf("The available rooms are:\n");
    for(int i=0; i<n; i++)
    {
        if(strcmp(x,z[i].status)==0)
        {
            printf("Room number:%d Category:%s Price:%d\n",z[i].roomNumber,z[i].category,z[i].price);
        }
    }
}
void checkIn(Reservation *r, room *z, int n2,int c)
{
    int i = 0,found=0;
    char p[20];

    Reservation checkInData;
    printf("Enter Reservation ID: ");
    fgets(p, sizeof(p), stdin);
    p[strcspn(p, "\n")] = 0;
    for (i = 0; p[i] != '\0'; i++)
    {
        if (!(p[i] >= '0' && p[i] <= '9'))



        {
            printf("Invalid num, re-enter: \n");
            fgets(p, sizeof(p), stdin);
            p[strcspn(p, "\n")] = 0;
            i = -1;
        }
    }
    checkInData.reservationID= atoi(p);
    printf("Enter the room number: ");
    fgets(p, sizeof(p), stdin);
    p[strcspn(p, "\n")] = 0;
    for (i = 0; p[i] != '\0'; i++)
    {
        if (!(p[i] >= '0' && p[i] <= '9'))



        {
            setColor(12,0);
            printf("Invalid num, re-enter: \n");
            setColor(7,0);
            fgets(p, sizeof(p), stdin);
            p[strcspn(p, "\n")] = 0;
            i = -1;
        }
    }
    checkInData.roomNumber= atoi(p);
    printf("Enter the Customer name: ");


    fgets(checkInData.customerName, sizeof(checkInData.customerName), stdin);
    checkInData.customerName[strcspn(checkInData.customerName, "\n")] = 0;
    for (i = 0; checkInData.customerName[i] != '\0'; i++)
    {
        if (!((checkInData.customerName[i] >= 'A' && checkInData.customerName[i] <= 'Z') ||
                (checkInData.customerName[i] >= 'a' && checkInData.customerName[i] <= 'z') ||
                checkInData.customerName[i] == ' '))
        {
            printf("Invalid name, re-enter: \n");
            fgets(checkInData.customerName, sizeof(checkInData.customerName), stdin);
            checkInData.customerName[strcspn(checkInData.customerName, "\n")] = 0;
            i = -1;
        }
    }
    checkInData.customerName[strcspn(checkInData.customerName,"\n")]=0;
    printf("Enter the Customer national ID: ");
    fgets(checkInData.nationalID, sizeof(checkInData.nationalID), stdin);
    checkInData.nationalID[strcspn(checkInData.nationalID, "\n")] = 0;
    for (i = 0; checkInData.nationalID[i] != '\0'; i++)
    {
        if (!(checkInData.nationalID[i] >= '0' && checkInData.nationalID[i] <= '9'))
        {
            printf("Invalid national ID, re-enter: \n");
            fgets(checkInData.nationalID, sizeof(checkInData.nationalID), stdin);
            checkInData.nationalID[strcspn(checkInData.nationalID, "\n")] = 0;
            i = -1;
        }
    }

    printf("Enter the number of nights: ");
    fgets(p, sizeof(p), stdin);
    p[strcspn(p, "\n")] = 0;
    for (i = 0; p[i] != '\0'; i++)
    {
        if (!(p[i] >= '0' && p[i] <= '9'))



        {
            printf("Invalid num, re-enter: \n");
            fgets(p, sizeof(p), stdin);
            p[strcspn(p, "\n")] = 0;
            i = -1;
        }
    }
    checkInData.nights= atoi(p);
    printf("Enter the check-in month (in MM format): ");
    while (1)
    {
        scanf("%s", checkInData.cid.month);
        if (strcmp(checkInData.cid.month, "01") == 0 || strcmp(checkInData.cid.month, "02") == 0 ||
                strcmp(checkInData.cid.month, "03") == 0 || strcmp(checkInData.cid.month, "04") == 0 ||
                strcmp(checkInData.cid.month, "05") == 0 || strcmp(checkInData.cid.month, "06") == 0 ||
                strcmp(checkInData.cid.month, "07") == 0 || strcmp(checkInData.cid.month, "08") == 0 ||
                strcmp(checkInData.cid.month, "09") == 0 || strcmp(checkInData.cid.month, "10") == 0 ||
                strcmp(checkInData.cid.month, "11") == 0 || strcmp(checkInData.cid.month, "12") == 0)
        {
            break;
        }
        else
        {
            setColor(12,0);
            printf("Invalid month, re-enter (in MM format): ");
            setColor(7,0);
        }
    }

    printf("Enter the check-in day (in DD format): ");
    while (1)
    {
        scanf("%s", checkInData.cid.day);
        int day = atoi(checkInData.cid.day);
        int month = atoi(checkInData.cid.month);
        if (strlen(checkInData.cid.day) == 2 && isdigit(checkInData.cid.day[0]) && isdigit(checkInData.cid.day[1]))
        {
            if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
                    month == 10 || month == 12) && (day >= 1 && day <= 31))
            {
                break;
            }
            else if ((month == 4 || month == 6 || month == 9 || month == 11) &&
                     (day >= 1 && day <= 30))
            {
                break;
            }
            else if (month == 2 && (day >= 1 && day <= 28))
            {
                break;
            }
        }
        printf("Invalid day for given month, re-enter (in DD format): ");
    }

    printf("Enter the check-in year (in YYYY format): ");
    while (1)
    {
        scanf("%s", checkInData.cid.year);
        if (strlen(checkInData.cid.year) == 4 && isdigit(checkInData.cid.year[0]) && isdigit(checkInData.cid.year[1]) &&
                isdigit(checkInData.cid.year[2]) && isdigit(checkInData.cid.year[3]))
        {
            break;
        }
        else
        {
            setColor(12,0);
            printf("Invalid year, re-enter (in YYYY format): ");
            setColor(7,0);
        }
    }

    printf("Enter the email: ");
    while (getchar() != '\n');
    fgets(checkInData.email, sizeof(checkInData.email), stdin);
    checkInData.email[strcspn(checkInData.email, "\n")] = 0;
    while (strchr(checkInData.email, '@') == NULL || strchr(checkInData.email, '.') == NULL)
    {
        printf("Invalid email, re-enter: \n");
        fgets(checkInData.email, sizeof(checkInData.email), stdin);
        checkInData.email[strcspn(checkInData.email, "\n")] = 0;
    }

    printf("Enter the mobile number: ");
    while (1)
    {
        fgets(checkInData.mobileNumber, sizeof(checkInData.mobileNumber), stdin);
        checkInData.mobileNumber[strcspn(checkInData.mobileNumber, "\n")] = 0;

        if (strlen(checkInData.mobileNumber) == 11 && checkInData.mobileNumber[0] == '0' &&
                checkInData.mobileNumber[1] == '1')
        {
            int valid = 1;
            for (i = 0; i < 11; i++)
            {
                if (checkInData.mobileNumber[i] < '0' || checkInData.mobileNumber[i] > '9')
                {
                    valid = 0;
                    break;
                }
            }
            if (valid) break;
        }
        printf("Invalid mobile number, re-enter: \n");
    }
    strcpy(checkInData.status,"unconfirmed");



    i = 0;
    while(!(atoi(checkInData.cid.year) > atoi(r[i].cid.year) ||
            (atoi(checkInData.cid.year) == atoi(r[i].cid.year) && atoi(checkInData.cid.month) > atoi(r[i].cid.month)) ||
            (atoi(checkInData.cid.year) == atoi(r[i].cid.year) && atoi(r[i].cid.month) == atoi(checkInData.cid.month) && atoi(checkInData.cid.day) >= atoi(r[i].cid.day))))
    {
        printf("Invalid check-in date\n");
        printf("Enter the check-in date (DD): ");
        scanf("%s", checkInData.cid.day);
        printf("Enter the check-in date (MM): ");
        scanf("%s", checkInData.cid.month);
        printf("Enter the check-in date (YYYY): ");
        scanf("%s", checkInData.cid.year);
    }

    for (i = 0; i <c; i++)
    {
        if (checkInData.reservationID == r[i].reservationID &&
                checkInData.roomNumber == r[i].roomNumber &&
                strcmp(checkInData.customerName, r[i].customerName) == 0 &&
                strcmp(checkInData.nationalID, r[i].nationalID) == 0 &&
                checkInData.nights == r[i].nights &&
                strcmp(checkInData.cid.day, r[i].cid.day) == 0 &&
                strcmp(checkInData.cid.month, r[i].cid.month) == 0 &&
                strcmp(checkInData.cid.year, r[i].cid.year) == 0 &&
                strcmp(checkInData.email, r[i].email) == 0 &&
                strcmp(checkInData.mobileNumber, r[i].mobileNumber) == 0)
        {
            if (strcmp(r[i].status, "confirmed") == 0)
            {
                printf("Reservation already confirmed\n");
                return;
            }
            else
            {
                found = 1;
                setColor(10,0);
                printf("Checked in successfully\n");
                setColor(7,0);
                char a[100];
                printf("Enter 1 to save or any key to exit: ");
                scanf("%s",a);
                if(strcmp(a,"1")==0)
                {
                    strcpy(r[i].status, "confirmed");

                    FILE *f2=fopen("Reservations.txt","w");
                    for(i=0; i<c; i++)
                    {
                        fprintf(f2,"%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n", r[i].reservationID, r[i].roomNumber, r[i].status,
                                r[i].customerName, r[i].nationalID, r[i].nights,
                                r[i].cid.day, r[i].cid.month, r[i].cid.year,
                                r[i].email, r[i].mobileNumber);
                    }
                    fclose(f2);
                    for(i=0; i<n2; i++)
                    {
                        if(z[i].roomNumber==checkInData.roomNumber)
                        {
                            strcpy(z[i].status, "Reserved");
                            FILE *f1=fopen("rooms.txt","w");
                            for(i=0; i<n2; i++)
                            {
                                fprintf(f1, "%d %s %s %d\n", z[i].roomNumber, z[i].status, z[i].category, z[i].price);
                            }
                            fclose(f1);
                        }
                    }


                }

                return;
            }
        }
    }

    if (!found)
    {
        printf("Reservation not found\n");
    }

}
void Exit(int n,Reservation* r,room* z,int c)
{
    char s[500];
    setColor(14,0);
    printf("\nEnter 0 to ");
    setColor(12,0);
    printf("EXIT");
    setColor(14,0);
    printf(", or enter 1 to return to Main Menu:");
    setColor(7,0);
    scanf("%s",s);
    if(strcmp(s,"0")==0)
        exit(0);
    else if(strcmp(s,"1")==0)
    {
        while (getchar() != '\n');
        mainMenu(n,r,z,c);
    }
    else
    {
        setColor(12,0);
        printf("Error\n");
        setColor(7,0);
        Exit(n,r,z,c);
    }
}
void Query(int n,Reservation* r,room* z)
{
    int room,i,found=0,j;
    char x[500];
    char customer[100],status[100],p[20];
    printf("Press 1 to retrieve reservation details and customer info\n");
    printf("Press 2 to view the current status and assigned guest information if reserved\n");
    printf("Press 3 to view all available or occupied rooms\n");
    printf("Enter 1 or 2 or 3:");
    scanf("%s",x);
    x[strcspn(x, "\n")]=0;
    if(strcmp(x,"1")==0)
    {
        printf("Enter the Customer name: ");
        while (getchar() != '\n');
        fgets(customer, sizeof(customer), stdin);
        customer[strcspn(customer, "\n")] = 0;
        for (i = 0; customer[i] != '\0'; i++)
        {
            if (!((customer[i] >= 'A' && customer[i] <= 'Z') ||
                    (customer[i] >= 'a' && customer[i] <= 'z') ||
                    customer[i] == ' '))
            {
                printf("Invalid name, re-enter: \n");
                fgets(customer, sizeof(customer), stdin);
                customer[strcspn(customer, "\n")] = 0;
                i = -1;
            }
        }
        customer[strcspn(customer,"\n")]=0;
        for(i=0; i<n; i++)
        {
            if(strcmp(customer, r[i].customerName) == 0)
            {
                printf("Reservation ID: %d\nRoom number: %d\nStatus: %s\nCheck-in Date: %s-%s-%s\nNights: %d\nNational ID: %s\nCustomer name: %s\nE-mail: %s\nMobile Number: %s\n",r[i].reservationID,r[i].roomNumber,r[i].status,r[i].cid.day,r[i].cid.month,r[i].cid.year,r[i].nights,r[i].nationalID,r[i].customerName,r[i].email,r[i].mobileNumber);
                found=1;
                break;
            }

        }
        if(found==0)
            printf("Customer name not found!");
    }
    else if(strcmp(x,"2")==0)
    {
        int flag=0;
        printf("Room number:");
        while(getchar()!='\n');
        fgets(p, sizeof(p), stdin);
        p[strcspn(p, "\n")] = 0;
        for (i = 0; p[i] != '\0'; i++)
        {
            if (!(p[i] >= '0' && p[i] <= '9'))



            {
                printf("Invalid num, re-enter: \n");
                fgets(p, sizeof(p), stdin);
                p[strcspn(p, "\n")] = 0;
                i = -1;
            }
        }
        room= atoi(p);
        for(i=0; i<n; i++)
        {
            if(room==z[i].roomNumber)
            {
                flag=1;
                if(strcmp("Reserved",z[i].status)==0)
                {
                    printf("Reserved\n");
                    for(j=0; j<n; j++)
                    {
                        if(room==r[j].roomNumber)
                            printf("Reservation ID: %d\nRoom number: %d\nStatus: %s\nCheck-in Date: %s-%s-%s\nNights: %d\nNational ID: %s\nCustomer name: %s\nE-mail: %s\nMobile Number: %s\n",r[j].reservationID,r[j].roomNumber,r[j].status,r[j].cid.day,r[j].cid.month,r[j].cid.year,r[j].nights,r[j].nationalID,r[j].customerName,r[j].email,r[j].mobileNumber);
                    }
                }
                else if(strcmp("Available",z[i].status)==0)
                {
                    printf("Available\n");

                }
            }
            if(flag==1)
                break;
        }
        if (flag==0)
            printf("Room not found");
    }
    else if(strcmp(x,"3")==0)
    {
        printf("Choose 'Available' or 'Occupied':");
        while (getchar() != '\n');
        fgets(status, sizeof(status), stdin);
        status[strcspn(status, "\n")] = 0;
        for (i = 0; status[i] != '\0'; i++)
        {
            if (!((status[i] >= 'A' && status[i] <= 'Z') ||
                    (status[i] >= 'a' && status[i] <= 'z') ||
                    status[i] == ' '))
            {
                printf("Invalid name, re-enter: \n");
                fgets(status, sizeof(status), stdin);
                status[strcspn(status, "\n")] = 0;
                i = -1;
            }
        }
        status[strcspn(status,"\n")]=0;

        if(strcasecmp(status,"Occupied")==0)
        {
            strcpy(status,"Reserved");
            printf("The reserved rooms are:\n");
            for(int i=0; i<n; i++)
            {
                if(strcmp(status,z[i].status)==0)
                {
                    printf("Room number:%d Category:%s Price:%d\n",z[i].roomNumber,z[i].category,z[i].price);
                }
            }
        }
        else if(strcasecmp(status,"Available")==0)
        {
            TRA(n,z);
        }
        else
        {
            printf("Invalid entry");
        }

    }
    else
    {
        setColor(12,0);
        printf("Error\n");
        setColor(7,0);
        Query(n,r,z);
    }

}
void cancelReservation(int *c,Reservation *r,int n,room* z )
{
    int x;
    char p[500];
    printf("Enter 1 to enter reservation ID or 0 to enter room number:");
    fgets(p, sizeof(p), stdin);
    p[strcspn(p, "\n")] = 0;

    int i, j, flag = 0, IDcancel, roomcancel;
    if (strcmp(p,"1")==0)
    {
        printf("Enter Reservation ID to cancel: ");
        scanf("%d", &IDcancel);
        for (i = 0; i < *c; i++)
        {
            if (r[i].reservationID == IDcancel)
            {
                flag = 1;
                if (strcmp(r[i].status, "confirmed") == 0)
                {
                    printf("Cannot cancel, status is confirmed.\n");
                    return;
                }
                for (j = 0; j < n; j++)
                {
                    if (z[j].roomNumber == r[i].roomNumber)
                    {
                        strcpy(z[j].status, "Available");
                        break;
                    }
                }
                for (j = i; j < *c - 1; j++)
                {
                    r[j] = r[j + 1];
                }
                (*c)--;
                break;
            }
        }
        if (flag == 0)
        {
            printf("Reservation not found.\n");
        }

    }
    else if (strcmp(p,"0")==0)
    {
        printf("Enter room number to cancel: ");
        scanf("%d", &roomcancel);
        for (i = 0; i < *c; i++)
        {
            if (r[i].roomNumber == roomcancel)
            {
                flag = 1;
                if (strcmp(r[i].status, "confirmed") == 0)
                {
                    printf("Cannot cancel, status is confirmed.\n");
                    return;
                }
                for (j = 0; j < n; j++)
                {
                    if (z[j].roomNumber == r[i].roomNumber)
                    {
                        strcpy(z[j].status, "Available");
                        break;
                    }
                }
                for (j = i; j < *c - 1; j++)
                {
                    r[j] = r[j + 1];
                }
                (*c)--;
                break;
            }
        }
        if (flag == 0)
        {
            printf("Reservation not found.\n");
        }

    }
    else
    {
        setColor(12,0);
        printf("Error\n");
        setColor(7,0);
        cancelReservation(&c,r,n,z);
    }

    char a;
    printf("Enter 1 to save and any other key to unsave and continue:");
    while (getchar() != '\n');
    scanf("%c",&a);
    if (a=='1')
    {
        FILE *f2=fopen("Reservations.txt","w");
        FILE *f1 = fopen("rooms.txt", "w");
        for (i = 0; i < n; i++)
        {
            fprintf(f1, "%d %s %s %d\n", z[i].roomNumber, z[i].status, z[i].category, z[i].price);
        }
        for(i=0; i<*c; i++)
        {
            fprintf(f2,"%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n", r[i].reservationID, r[i].roomNumber, r[i].status,
                    r[i].customerName, r[i].nationalID, r[i].nights,
                    r[i].cid.day, r[i].cid.month, r[i].cid.year,
                    r[i].email, r[i].mobileNumber);
        }
        fclose(f1);
        fclose(f2);
    }
    else
        *c=Load(r,n);
    loadrooms(z,n);
}
void checkout(int *c,Reservation *r,int n,room* z )
{


    int i, j, flag = 0,roomcancel, price;



    printf("Enter room number to  checkout: ");
    scanf("%d", &roomcancel);

    for (i = 0; i < *c; i++)
    {
        if (r[i].roomNumber == roomcancel)
        {
            flag = 1;
            if (strcmp(r[i].status, "unconfirmed") == 0)
            {
                printf("Cannot cancel, status is unconfirmed.\n");
                return;
            }

            for (j = 0; j < n; j++)
            {
                if (z[j].roomNumber == r[i].roomNumber)
                {
                    strcpy(z[j].status, "Available");
                    break;
                }
            }
            printf("total price for %d nights is: %d \n",r[i].nights,r[i].nights*z[j].price);
            for (j = i; j < *c - 1; j++)
            {
                r[j] = r[j + 1];
            }
            (*c)--;
            break;
        }
    }
    if (flag == 0)
    {
        printf("No confirmed check-ins for this room.\n");
    }



    char a[100];
    printf("Enter 1 to save and any other key to unsave and continue:");
    while (getchar() != '\n');
    scanf("%s",a);
    if (strcmp(a,"1")==0)
    {
        FILE *f2=fopen("Reservations.txt","w");
        FILE *f1 = fopen("rooms.txt", "w");
        for (i = 0; i < n; i++)
        {
            fprintf(f1, "%d %s %s %d\n", z[i].roomNumber, z[i].status, z[i].category, z[i].price);
        }
        for(i=0; i<*c; i++)
        {
            fprintf(f2,"%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n", r[i].reservationID, r[i].roomNumber, r[i].status,
                    r[i].customerName, r[i].nationalID, r[i].nights,
                    r[i].cid.day, r[i].cid.month, r[i].cid.year,
                    r[i].email, r[i].mobileNumber);
        }
        fclose(f1);
        fclose(f2);
    }
    else
        *c=Load(r,n);
    loadrooms(z,n);
}
void report (Reservation *r,int c)
{
    date x;
    printf("Enter the check-in month (in MM format): ");
    while (1)
    {
        scanf("%s",x.month);
        if (strcmp(x.month, "01") == 0 || strcmp(x.month, "02") == 0 ||
                strcmp(x.month, "03") == 0 || strcmp(x.month, "04") == 0 ||
                strcmp(x.month, "05") == 0 || strcmp(x.month, "06") == 0 ||
                strcmp(x.month, "07") == 0 || strcmp(x.month, "08") == 0 ||
                strcmp(x.month, "09") == 0 || strcmp(x.month, "10") == 0 ||
                strcmp(x.month, "11") == 0 || strcmp(x.month, "12") == 0)
        {
            break;
        }
        else
        {
            setColor(12,0);
            printf("Invalid month, re-enter (in MM format): ");
            setColor(7,0);
        }
    }

    printf("Enter the check-in day (in DD format): ");
    while (1)
    {
        scanf("%s", x.day);
        int day = atoi(x.day);
        int month = atoi(x.month);
        if (strlen(x.day) == 2 && isdigit(x.day[0]) && isdigit(x.day[1]))
        {
            if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
                    month == 10 || month == 12) && (day >= 1 && day <= 31))
            {
                break;
            }
            else if ((month == 4 || month == 6 || month == 9 || month == 11) &&
                     (day >= 1 && day <= 30))
            {
                break;
            }
            else if (month == 2 && (day >= 1 && day <= 28))
            {
                break;
            }
        }
        setColor(12,0);
        printf("Invalid day for given month, re-enter (in DD format): ");
        setColor(7,0);
    }

    printf("Enter the check-in year (in YYYY format): ");
    while (1)
    {
        scanf("%s", x.year);
        if (strlen(x.year) == 4 && isdigit(x.year[0]) && isdigit(x.year[1]) &&
                isdigit(x.year[2]) && isdigit(x.year[3]))
        {
            break;
        }
        else
        {
            setColor(12,0);
            printf("Invalid year, re-enter (in YYYY format): ");
            setColor(7,0);
        }
    }
    int i,flag=0;
    for (i=0; i<c; i++)
    {
        if (strcmp(r[i].cid.day,x.day)==0&&strcmp(r[i].cid.month,x.month)==0&&strcmp(r[i].cid.year,x.year)==0)
        {
            printf("Customer Name:%s\nMobile Number:%s\nNational ID:%s\nEmail:%s",r[i].customerName,r[i].mobileNumber,r[i].nationalID,r[i].email);
            flag=1;
            break;
        }
    }
    if (flag==0)
       {
           setColor(12,0);
    printf("Date Not Found");
    setColor(7,0);
       }
}
void sort(Reservation *r, int c)
{
    int i,j;
    Reservation temp;
    for (i = 0; i<c-1; i++)
    {
        for (j = 0; j<c-i-1; j++)
        {

            int y1 = atoi(r[j].cid.year);
            int m1 = atoi(r[j].cid.month);
            int d1 = atoi(r[j].cid.day);
            int y2 = atoi(r[j+1].cid.year);
            int m2 = atoi(r[j+1].cid.month);
            int d2 = atoi(r[j+1].cid.day);

            if (y1>y2|| (y1==y2&&m1>m2)||(y1==y2&& m1==m2&& d1>d2))
            {
                temp = r[j];
                r[j] = r[j + 1];
                r[j + 1] = temp;
            }
        }
    }
    FILE *f2 = fopen("Reservations.txt", "w");
    for(i=0; i<c; i++)
    {
        fprintf(f2,"%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n", r[i].reservationID, r[i].roomNumber, r[i].status,
                r[i].customerName, r[i].nationalID, r[i].nights,
                r[i].cid.day, r[i].cid.month, r[i].cid.year,
                r[i].email, r[i].mobileNumber);
    }
    fclose(f2);
}
void editReservation(Reservation* r,int c,room* z,int n)
{
    int x,i,j,l1,l2,flag=0,h,flag3=0;
    char p[20];
    char y[50];
        printf("Enter 0 for reservationID or Enter 1 for Room number:");
        scanf("%s",y);
    if(strcmp(y,"0")==0)
    {

        printf("Enter Reservation ID: ");
        while (getchar() != '\n');
        fgets(p, sizeof(p), stdin);
        p[strcspn(p, "\n")] = 0;
        for (i = 0; p[i] != '\0'; i++)
        {
            if (!(p[i] >= '0' && p[i] <= '9'))
            {
                printf("Invalid num, re-enter: \n");
                fgets(p, sizeof(p), stdin);
                p[strcspn(p, "\n")] = 0;
                i = -1;
            }
        }
        x= atoi(p);
        for(i=0; i<c; i++)
        {
            if(r[i].reservationID==x)
            {
                flag=1;

                l1=i;
                for(j=0; j<n; j++)
                {
                    if(r[l1].roomNumber==z[j].roomNumber)
                    {
                        l2=j;

                    }
                }
                break;
            }
        }
        if(flag==1)
        {
            printf("Enter the Customer name: ");
            fgets(r[l1].customerName, sizeof(r[l1].customerName), stdin);
            r[l1].customerName[strcspn(r[l1].customerName, "\n")] = 0;
            for (i = 0; r[l1].customerName[i] != '\0'; i++)
            {
                if (!((r[l1].customerName[i] >= 'A' && r[l1].customerName[i] <= 'Z') ||
                        (r[l1].customerName[i] >= 'a' && r[l1].customerName[i] <= 'z') ||
                        r[l1].customerName[i] == ' '))
                {
                    printf("Invalid name, re-enter: \n");
                    fgets(r[l1].customerName, sizeof(r[l1].customerName), stdin);
                    r[l1].customerName[strcspn(r[l1].customerName, "\n")] = 0;
                    i = -1;
                }
            }
            r[l1].customerName[strcspn(r[l1].customerName,"\n")] = 0;

            printf("Enter the mobile number: ");
            while (1)
            {
                fgets(r[l1].mobileNumber, sizeof(r[l1].mobileNumber), stdin);
                r[l1].mobileNumber[strcspn(r[l1].mobileNumber, "\n")] = 0;

                if (strlen(r[l1].mobileNumber) == 11 && r[l1].mobileNumber[0] == '0' &&
                        r[l1].mobileNumber[1] == '1')
                {
                    int valid = 1;
                    for (i = 0; i < 11; i++)
                    {
                        if (r[l1].mobileNumber[i] < '0' || r[l1].mobileNumber[i] > '9')
                        {
                            valid = 0;
                            break;
                        }
                    }
                    if (valid) break;
                }
                printf("Invalid mobile number, re-enter: \n");
            }

            printf("Enter the email: ");
            fgets(r[l1].email, sizeof(r[l1].email), stdin);
            r[l1].email[strcspn(r[l1].email, "\n")] = 0;
            while (strchr(r[l1].email, '@') == NULL || strchr(r[l1].email, '.') == NULL)
            {
                printf("Invalid email, re-enter: \n");
                fgets(r[l1].email, sizeof(r[l1].email), stdin);
                r[l1].email[strcspn(r[l1].email, "\n")] = 0;
            }

            printf("Enter the check-in month (in MM format): ");
            while (1)
            {
                scanf("%s", r[l1].cid.month);
                if (strcmp(r[l1].cid.month, "01") == 0 || strcmp(r[l1].cid.month, "02") == 0 ||
                        strcmp(r[l1].cid.month, "03") == 0 || strcmp(r[l1].cid.month, "04") == 0 ||
                        strcmp(r[l1].cid.month, "05") == 0 || strcmp(r[l1].cid.month, "06") == 0 ||
                        strcmp(r[l1].cid.month, "07") == 0 || strcmp(r[l1].cid.month, "08") == 0 ||
                        strcmp(r[l1].cid.month, "09") == 0 || strcmp(r[l1].cid.month, "10") == 0 ||
                        strcmp(r[l1].cid.month, "11") == 0 || strcmp(r[l1].cid.month, "12") == 0)
                {
                    break;
                }
                else
                {
                    printf("Invalid month, re-enter (in MM format): ");
                }
            }

            printf("Enter the check-in day (in DD format): ");
            while (1)
            {
                scanf("%s", r[l1].cid.day);
                int day = atoi(r[l1].cid.day);
                int month = atoi(r[l1].cid.month);
                if (strlen(r[l1].cid.day) == 2 && isdigit(r[l1].cid.day[0]) && isdigit(r[l1].cid.day[1]))
                {
                    if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
                            month == 10 || month == 12) && (day >= 1 && day <= 31))
                    {
                        break;
                    }
                    else if ((month == 4 || month == 6 || month == 9 || month == 11) &&
                             (day >= 1 && day <= 30))
                    {
                        break;
                    }
                    else if (month == 2 && (day >= 1 && day <= 28))
                    {
                        break;
                    }
                }
                printf("Invalid day for given month, re-enter (in DD format): ");
            }

            printf("Enter the check-in year (in YYYY format): ");
            while (1)
            {
                scanf("%s", r[l1].cid.year);
                if (strlen(r[l1].cid.year) == 4 && isdigit(r[l1].cid.year[0]) && isdigit(r[l1].cid.year[1]) &&
                        isdigit(r[l1].cid.year[2]) && isdigit(r[l1].cid.year[3]))
                {
                    break;
                }
                else
                {
                    printf("Invalid year, re-enter (in YYYY format): ");
                }
            }
            char f[500],b[20];
            int flag2 = 0;
            while (getchar() != '\n');
            printf("Would you like to change room category?(type 1 if yes or any other key if no):");
            scanf("%s",&b);
            if(strcmp(b,"1")==0)
            {
                strcpy(z[l2].status,"Available");

                do
                {
                    printf("Enter the category: ");
                    while (getchar() != '\n');

                    fgets(f, sizeof(f), stdin);
                    f[strcspn(f, "\n")] = 0;
                    for (i = 0; i < n; i++)
                    {
                        if (strcasecmp(f, z[i].category) == 0 && strcasecmp(z[i].status, "Available") == 0)
                        {
                            r[l1].roomNumber = z[i].roomNumber;
                            strcpy(z[i].status, "Reserved");
                            flag2 = 1;
                            break;
                        }
                    }
                    if (!flag2)
                    {
                        printf("Category unavailable, please re-enter.\n");
                    }
                }
                while (!flag2);
            }
            printf("Enter the number of nights: ");
             while (getchar() != '\n');
            fgets(p, sizeof(p), stdin);
            p[strcspn(p, "\n")] = 0;
            for (i = 0; p[i] != '\0'; i++)
            {
                if (!(p[i] >= '0' && p[i] <= '9'))
                {
                    printf("Invalid num, re-enter: \n");
                    fgets(p, sizeof(p), stdin);
                    p[strcspn(p, "\n")] = 0;
                    i = -1;
                }
            }
            r[l1].nights = atoi(p);

        }
        else
        {
            printf("Reservation ID not found");
        }

    }
    else if(strcmp(y,"1")==0)
    {
        printf("enter room number:");
         while (getchar() != '\n');
        fgets(p, sizeof(p), stdin);
        p[strcspn(p, "\n")] = 0;
        for (i = 0; p[i] != '\0'; i++)
        {
            if (!(p[i] >= '0' && p[i] <= '9'))
            {
                printf("Invalid num, re-enter: \n");
                fgets(p, sizeof(p), stdin);
                p[strcspn(p, "\n")] = 0;
                i = -1;
            }
        }
        h= atoi(p);
        for(i=0; i<c; i++)
        {
            if(r[i].roomNumber==h)

            {
                flag3=1;

                l1=i;

                for(j=0; j<n; j++)
                {

                    if(r[l1].roomNumber==z[j].roomNumber)
                    {
                        l2=j;

                    }


                }
                break;


            }
        }
        if(flag3==1)
        {
            printf("Enter the Customer name: ");
            fgets(r[l1].customerName, sizeof(r[l1].customerName), stdin);
            r[l1].customerName[strcspn(r[l1].customerName, "\n")] = 0;
            for (i = 0; r[l1].customerName[i] != '\0'; i++)
            {
                if (!((r[l1].customerName[i] >= 'A' && r[l1].customerName[i] <= 'Z') ||
                        (r[l1].customerName[i] >= 'a' && r[l1].customerName[i] <= 'z') ||
                        r[l1].customerName[i] == ' '))
                {
                    printf("Invalid name, re-enter: \n");
                    fgets(r[l1].customerName, sizeof(r[l1].customerName), stdin);
                    r[l1].customerName[strcspn(r[l1].customerName, "\n")] = 0;
                    i = -1;
                }
            }
            r[l1].customerName[strcspn(r[l1].customerName,"\n")] = 0;

            printf("Enter the mobile number: ");
            while (1)
            {
                fgets(r[l1].mobileNumber, sizeof(r[l1].mobileNumber), stdin);
                r[l1].mobileNumber[strcspn(r[l1].mobileNumber, "\n")] = 0;

                if (strlen(r[l1].mobileNumber) == 11 && r[l1].mobileNumber[0] == '0' &&
                        r[l1].mobileNumber[1] == '1')
                {
                    int valid = 1;
                    for (i = 0; i < 11; i++)
                    {
                        if (r[l1].mobileNumber[i] < '0' || r[l1].mobileNumber[i] > '9')
                        {
                            valid = 0;
                            break;
                        }
                    }
                    if (valid) break;
                }
                printf("Invalid mobile number, re-enter: \n");
            }

            printf("Enter the email: ");
            fgets(r[l1].email, sizeof(r[l1].email), stdin);
            r[l1].email[strcspn(r[l1].email, "\n")] = 0;
            while (strchr(r[l1].email, '@') == NULL || strchr(r[l1].email, '.') == NULL)
            {
                printf("Invalid email, re-enter: \n");
                fgets(r[l1].email, sizeof(r[l1].email), stdin);
                r[l1].email[strcspn(r[l1].email, "\n")] = 0;
            }

            printf("Enter the check-in month (in MM format): ");
            while (1)
            {
                scanf("%s", r[l1].cid.month);
                if (strcmp(r[l1].cid.month, "01") == 0 || strcmp(r[l1].cid.month, "02") == 0 ||
                        strcmp(r[l1].cid.month, "03") == 0 || strcmp(r[l1].cid.month, "04") == 0 ||
                        strcmp(r[l1].cid.month, "05") == 0 || strcmp(r[l1].cid.month, "06") == 0 ||
                        strcmp(r[l1].cid.month, "07") == 0 || strcmp(r[l1].cid.month, "08") == 0 ||
                        strcmp(r[l1].cid.month, "09") == 0 || strcmp(r[l1].cid.month, "10") == 0 ||
                        strcmp(r[l1].cid.month, "11") == 0 || strcmp(r[l1].cid.month, "12") == 0)
                {
                    break;
                }
                else
                {
                    printf("Invalid month, re-enter (in MM format): ");
                }
            }

            printf("Enter the check-in day (in DD format): ");
            while (1)
            {
                scanf("%s", r[l1].cid.day);
                int day = atoi(r[l1].cid.day);
                int month = atoi(r[l1].cid.month);
                if (strlen(r[l1].cid.day) == 2 && isdigit(r[l1].cid.day[0]) && isdigit(r[l1].cid.day[1]))
                {
                    if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
                            month == 10 || month == 12) && (day >= 1 && day <= 31))
                    {
                        break;
                    }
                    else if ((month == 4 || month == 6 || month == 9 || month == 11) &&
                             (day >= 1 && day <= 30))
                    {
                        break;
                    }
                    else if (month == 2 && (day >= 1 && day <= 28))
                    {
                        break;
                    }
                }
                printf("Invalid day for given month, re-enter (in DD format): ");
            }

            printf("Enter the check-in year (in YYYY format): ");
            while (1)
            {
                scanf("%s", r[l1].cid.year);
                if (strlen(r[l1].cid.year) == 4 && isdigit(r[l1].cid.year[0]) && isdigit(r[l1].cid.year[1]) &&
                        isdigit(r[l1].cid.year[2]) && isdigit(r[l1].cid.year[3]))
                {
                    break;
                }
                else
                {
                    printf("Invalid year, re-enter (in YYYY format): ");
                }
            }
            char f[500],b[20];
            int flag2 = 0;
            while (getchar() != '\n');
            printf("Would you like to change room category?(type 1 if yes or any other key if no):");
            scanf("%s",&b);
            if(strcmp(b,"1")==0)
            {
                strcpy(z[l2].status,"Available");


                do
                {
                    printf("Enter the category: ");
                    while (getchar() != '\n');
                    fgets(f, sizeof(f), stdin);
                    f[strcspn(f, "\n")] = 0;
                    for (i = 0; i < n; i++)
                    {
                        if (strcasecmp(f, z[i].category) == 0 && strcasecmp(z[i].status, "Available") == 0)
                        {
                            r[l1].roomNumber = z[i].roomNumber;
                            strcpy(z[i].status, "Reserved");
                            flag2 = 1;
                            break;
                        }
                    }
                    if (!flag2)
                    {
                        printf("Category unavailable, please re-enter.\n");
                    }
                }
                while (!flag2);
            }
            printf("Enter the number of nights: ");
             while (getchar() != '\n');
            fgets(p, sizeof(p), stdin);
            p[strcspn(p, "\n")] = 0;
            printf(" ");
            for (i = 0; p[i] != '\0'; i++)
            {
                if (!(p[i] >= '0' && p[i] <= '9'))
                {
                    printf("Invalid num, re-enter: \n");
                    fgets(p, sizeof(p), stdin);
                    p[strcspn(p, "\n")] = 0;
                    i = -1;
                }
            }
            r[l1].nights = atoi(p);

        }
        else
        {
            printf("Reservation ID not found");
        }
    }
    else
    {
        printf("Error\n");
        editReservation(r,c,z,n);
    }

    char a[100];
    if(flag==1||flag3==1)
    {
        printf("Enter 1 to save or any key to exit: ");
        scanf("%s",a);
        if(strcmp(a,"1")==0)
        {
            FILE *f2=fopen("Reservations.txt","w");
            for(i=0; i<c; i++)
            {
                fprintf(f2,"%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n", r[i].reservationID, r[i].roomNumber, r[i].status,
                        r[i].customerName, r[i].nationalID, r[i].nights,
                        r[i].cid.day, r[i].cid.month, r[i].cid.year,
                        r[i].email, r[i].mobileNumber);
            }
            fclose(f2);
            FILE *f1=fopen("rooms.txt","w");
            for(i=0; i<n; i++)
            {
                fprintf(f1, "%d %s %s %d\n", z[i].roomNumber, z[i].status, z[i].category, z[i].price);
            }
            fclose(f1);




        }
    }
}
int main()
{
    int n2=CLRM();
    Reservation r[n2];
    room z[n2];
    userID();
    int c=Load(r,n2);
    sort(r,c);
    loadrooms(z,n2);
    mainMenu(n2,r,z,c);
    return 0;
}


