/* Josh Braegger CS3550
 Hotel Database Creation Script */
 
USE Master
 
IF  EXISTS (
    SELECT name
        FROM sys.databases
        WHERE name = 'Braegger_Hotel')

DROP Database Braegger_Hotel
GO

CREATE DATABASE Braegger_Hotel
ON Primary
(NAME = 'Braegger_Hotel', -- Virtual Internal Name
 FILENAME = 'C:\Program Files\Microsoft SQL Server\MSSQL.1\MSSQL\Data\Braegger_Hotel.mdf',
 SIZE = 2MB,
 MAXSIZE = 4MB, 
 FILEGROWTH = 500KB
)

LOG ON
(NAME = 'Braegger_Hotel_Log', -- Virtual Internal Name
 FILENAME = 'C:\Program Files\Microsoft SQL Server\MSSQL.1\MSSQL\Data\Braegger_Hotel.ldf',
 SIZE = 2MB,
 MAXSIZE = 4MB, 
 FILEGROWTH = 500KB
)
GO

USE Braegger_Hotel

CREATE TABLE CreditCard
( CreditCardID	smallint	IDENTITY(1,1) NOT NULL,
  GuestID	smallint NOT NULL,
  CCType	varchar(5)	NOT NULL,
  CCNumber	varchar(16)	NOT NULL,
  CCCompany	varchar(40),
  CCCardHolder	varchar(40)	NOT NULL,
  CCExpiration	smalldatetime	NOT NULL
)

CREATE TABLE Guest
( GuestID	smallint	IDENTITY(1500,1) NOT NULL,
  GuestFirst	varchar(20)	NOT NULL,
  GuestLast	varchar(20)	NOT NULL,
  GuestAddress1	varchar(30)	NOT NULL,
  GuestAddress2	varchar(10),
  GuestCity	varchar(20) NOT NULL,
  GuestState	char(2),
  GuestPostalCode	char(10),
  GuestCountry	varchar(20) NOT NULL,
  GuestPhone	varchar(20) NOT NULL,
  GuestEmail	varchar(30),
  GuestComments	varchar(200)
)

CREATE TABLE Reservation
( ReservationID	smallint IDENTITY(5000,1) NOT NULL,
  CreditCardID	smallint	NOT NULL,
  ReservationDate	smalldatetime	NOT NULL DEFAULT GetDate(),
  ReservationStatus	char(1)	NOT NULL
		DEFAULT 'A'
		CONSTRAINT DC_ReservationStatus CHECK (ReservationStatus IN ('A','B','C')) ,
  ReservationComments	varchar(200)
)

CREATE TABLE ReservationDetail
( ReservationDetailID	smallint	IDENTITY(1,1)	NOT NULL,
  RoomID	smallint	NOT NULL,
  ReservationID	smallint	NOT NULL,
  QuotedRate	smallmoney	NOT NULL,
  CheckinDate	smalldatetime	NOT NULL,
  Nights	tinyint	NOT NULL,
  [Status]	char(1)	NOT NULL
		DEFAULT 'A'
		CONSTRAINT DC_ReservationDetailStatus CHECK ([Status] IN ('A','B','C')),  
  Comments	varchar(200),
  DiscountID	smallint	NOT NULL
	DEFAULT 0
)

CREATE TABLE Room
( RoomID	smallint	IDENTITY(1,1)	NOT NULL,
  HotelRoomTypeID	smallint	NOT NULL,
  RoomNumber	varchar(5)	NOT NULL,
  RoomDescription	varchar(200)	NOT NULL,
  RoomSmoking	bit	NOT NULL,
  RoomBedConfiguration	varchar(30)	NOT NULL,
  HotelID	smallint	NOT NULL
)

CREATE TABLE HotelRoomType
( HotelRoomTypeID	smallint	IDENTITY(1,1)	NOT NULL,
  HotelID	smallint	NOT NULL,
  RoomType	varchar(20)	NOT NULL,
  RoomTypeRackRate smallmoney	NOT NULL,
  RoomTypeDescription	varchar(200)
)

CREATE TABLE ReservationDetailBilling
( ReservationBillingID	smallint	IDENTITY(1,1) NOT NULL,
  ReservationDetailID	smallint	NOT NULL,
  BillingCategoryID	smallint NOT NULL,
  BillingDescription	varchar(30)	NOT NULL,
  BillingAmount	smallmoney	NOT NULL,
  BillingItemQty	tinyint NOT NULL,
  BillingItemDate	smalldatetime	NOT NULL
)

CREATE TABLE Discount
( DiscountID	smallint	IDENTITY(1,1)	NOT NULL,
  DiscountDescription	varchar(50)	NOT NULL,
  DiscountExpiration	smalldatetime	NOT NULL,
  DiscountRules	varchar(100),
  DiscountPercent	decimal(4,2),
  DiscountAmount	smallmoney
)

CREATE TABLE Revenue
( RevenueID smallint IDENTITY(1,1)	NOT NULL,
  RevenueDate	smalldatetime	NOT NULL,
  RevenueAmount	smallmoney	NOT NULL,
  RevenueComments	varchar(200),
  HotelID	smallint	NOT NULL,
  RevenueCategoryID	smallint	NOT NULL
)

CREATE TABLE TaxRate
( TaxLocationID	smallint	IDENTITY(1,1)	NOT NULL,
  TaxDescription	varchar(30)	NOT NULL,
  RoomTaxRate	decimal(6,4)	NOT NULL,
  SalesTaxRate	decimal(6,4)	NOT NULL
)

CREATE TABLE Hotel
( HotelID	smallint	NOT NULL,
  HotelName	varchar(30)	NOT NULL,
  HotelAddress	varchar(30)	NOT NULL,
  HotelCity	varchar(20)	NOT NULL,
  HotelState	char(2),
  HotelCountry	varchar(20)	NOT NULL,
  HotelPostalCode	varchar(10)	NOT NULL,
  HotelStarRating	char(1),
  HotelPictureLink	varchar(100),
  TaxLocationID	smallint	NOT NULL
)

CREATE TABLE BillingCategory
( BillingCategoryID	smallint	NOT NULL    IDENTITY(1,1),
  BillingCategoryDescription	varchar(30)	NOT NULL
)

CREATE TABLE RevenueCategory
( RevenueCategoryID	smallint	NOT NULL	IDENTITY(1,1),
  RevenueCategoryDescription	varchar(30)	NOT NULL
)

ALTER TABLE CreditCard
ADD CONSTRAINT PK_CreditCardID
PRIMARY KEY (CreditCardID)

ALTER TABLE Guest
ADD CONSTRAINT PK_GuestID
PRIMARY KEY (GuestID)

ALTER TABLE Reservation
ADD CONSTRAINT PK_ReservationID
PRIMARY KEY (ReservationID)

ALTER TABLE ReservationDetail
ADD CONSTRAINT PK_ReservationDetailID
PRIMARY KEY (ReservationDetailID)

ALTER TABLE Room
ADD CONSTRAINT PK_RoomID
PRIMARY KEY (RoomID)

ALTER TABLE HotelRoomType
ADD CONSTRAINT PK_HotelRoomTypeID
PRIMARY KEY (HotelRoomTypeID)

ALTER TABLE ReservationDetailBilling
ADD CONSTRAINT PK_ReservationBillingID
PRIMARY KEY (ReservationBillingID)

ALTER TABLE Discount
ADD CONSTRAINT PK_DiscountID
PRIMARY KEY (DiscountID)

ALTER TABLE Revenue
ADD CONSTRAINT PK_RevenueID
PRIMARY KEY (RevenueID)

ALTER TABLE TaxRate
ADD CONSTRAINT PK_TaxLocationID
PRIMARY KEY (TaxLocationID)

ALTER TABLE Hotel
ADD CONSTRAINT PK_HotelID
PRIMARY KEY (HotelID)

ALTER TABLE BillingCategory
ADD CONSTRAINT PK_BillingCategoryID
PRIMARY KEY (BillingCategoryID)

ALTER TABLE RevenueCategory
ADD CONSTRAINT PK_RevenueCategoryID
PRIMARY KEY (RevenueCategoryID)

ALTER TABLE CreditCard
ADD CONSTRAINT FK_CreditCardMustHaveGuest
FOREIGN KEY (GuestID) REFERENCES Guest(GuestID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE Hotel
ADD CONSTRAINT FK_HotelMustHaveTaxRate
FOREIGN KEY (TaxLocationID) REFERENCES TaxRate(TaxLocationID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE HotelRoomType
ADD CONSTRAINT FK_HotelRoomTypeMustHaveHotel
FOREIGN KEY (HotelID) REFERENCES Hotel(HotelID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE Reservation
ADD CONSTRAINT FK_ReservationMustHaveCC
FOREIGN KEY (CreditCardID) REFERENCES CreditCard(CreditCardID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE ReservationDetail
ADD CONSTRAINT FK_ReservationDetailMustHaveRoom
FOREIGN KEY (RoomID) REFERENCES Room(RoomID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE ReservationDetail
ADD CONSTRAINT FK_ReservationDetailMustHaveDiscount
FOREIGN KEY (DiscountID) REFERENCES Discount(DiscountID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE ReservationDetail
ADD CONSTRAINT FK_ReservationDetailMustHaveReservation
FOREIGN KEY (ReservationID) REFERENCES Reservation(ReservationID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE ReservationDetailBilling
ADD CONSTRAINT FK_ReservationDetailBillingMustHaveReservationDetail
FOREIGN KEY (ReservationDetailID) REFERENCES ReservationDetail(ReservationDetailID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE ReservationDetailBilling
ADD CONSTRAINT FK_ReservationDetailBillingMustHaveBillingCategory
FOREIGN KEY (BillingCategoryID) REFERENCES BillingCategory(BillingCategoryID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE Revenue
ADD CONSTRAINT FK_RevenueMustHaveHotel
FOREIGN KEY (HotelID) REFERENCES Hotel(HotelID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE Revenue
ADD CONSTRAINT FK_RevenueMustHaveRevenueCategory
FOREIGN KEY (RevenueCategoryID) REFERENCES RevenueCategory(RevenueCategoryID)
ON UPDATE CASCADE
ON DELETE CASCADE

ALTER TABLE Room
ADD CONSTRAINT FK_RoomMustHaveRoomType
FOREIGN KEY (HotelRoomTypeID) REFERENCES HotelRoomType(HotelRoomTypeID)

ALTER TABLE Room
ADD CONSTRAINT FK_RoomMustHaveHotel
FOREIGN KEY (HotelID) REFERENCES Hotel(HotelID)