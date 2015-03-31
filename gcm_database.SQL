-- MySQL dump 10.13  Distrib 5.5.38, for debian-linux-gnu (x86_64)
--
-- Host: 127.0.0.1    Database: gcm
-- ------------------------------------------------------
-- Server version	5.6.19-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `CUSTOMER`
--

DROP TABLE IF EXISTS `CUSTOMER`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CUSTOMER` (
  `CUSTOMER_ID` int(11) NOT NULL AUTO_INCREMENT,
  `NAME_1` varchar(40) DEFAULT NULL,
  `NAME_2` varchar(40) DEFAULT NULL,
  `ADDR_BLK_NO` varchar(10) DEFAULT NULL,
  `ADDR_STREET_1` varchar(80) DEFAULT NULL,
  `ADDR_STREET_2` varchar(80) DEFAULT NULL,
  `ADDR_CITY` varchar(40) DEFAULT NULL,
  `ADDR_COUNTRY` varchar(40) DEFAULT 'Singapore',
  `PHONE` varchar(16) DEFAULT NULL,
  `MOBILE` varchar(16) DEFAULT NULL,
  `ACCOUNT_BAL` decimal(8,2) DEFAULT '0.00',
  `ADDR_POSTCODE` varchar(10) DEFAULT NULL,
  `UPDATEDON` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  UNIQUE KEY `CUSTOMER_ID` (`CUSTOMER_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CUSTOMER`
--

LOCK TABLES `CUSTOMER` WRITE;
/*!40000 ALTER TABLE `CUSTOMER` DISABLE KEYS */;
INSERT INTO `CUSTOMER` VALUES (1,'Chan Ah Kow','Mrs Claire Chan','455','Woodlands Street 31','#03-121',NULL,'Singapore','61234567','91234567',10.00,'730303','2015-03-31 11:15:19'),(2,'Tan Ah Kew',NULL,'24','Napier Road',NULL,NULL,'Singapore','68887777','98888888',0.00,'250111','2014-07-17 14:16:00'),(3,'Ravi',NULL,'12','Chancery Lane',NULL,NULL,'Singapore','65555555','95555555',0.00,'210001','2014-07-02 11:11:20'),(4,'Tan Ah Huat',NULL,'727','Ang Mo Kio Ave 6','#01-424',NULL,'Singapore','61234557',NULL,0.00,'560727','2015-03-14 10:43:38'),(5,'Wong Ah Teng',NULL,'28','Whampoa Drive',NULL,NULL,'Singapore','61111111',NULL,0.00,'320090','2014-07-17 11:54:15');
/*!40000 ALTER TABLE `CUSTOMER` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE /*!50017 DEFINER=`gcm_admin`@`%`*/ TRIGGER `CUSTOMER_ACCOUNT_UPDATE_TRG` AFTER UPDATE ON `CUSTOMER` FOR EACH ROW insert into CUSTOMER_AUDIT values(
    OLD.CUSTOMER_ID,
    OLD.ACCOUNT_BAL,
    NEW.ACCOUNT_BAL,
    CURRENT_TIMESTAMP);;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `CUSTOMER_AUDIT`
--

DROP TABLE IF EXISTS `CUSTOMER_AUDIT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CUSTOMER_AUDIT` (
  `CUSTOMER_ID` int(11) NOT NULL,
  `old_ACCOUNT_BAL` decimal(10,2) NOT NULL,
  `new_ACCOUNT_BAL` decimal(10,2) NOT NULL,
  `UPDATED_TIMESTAMP` timestamp NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CUSTOMER_AUDIT`
--

LOCK TABLES `CUSTOMER_AUDIT` WRITE;
/*!40000 ALTER TABLE `CUSTOMER_AUDIT` DISABLE KEYS */;
INSERT INTO `CUSTOMER_AUDIT` VALUES (1,0.00,10.00,'2015-03-31 11:15:19');
/*!40000 ALTER TABLE `CUSTOMER_AUDIT` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `JOB`
--

DROP TABLE IF EXISTS `JOB`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `JOB` (
  `JOB_ID` int(11) NOT NULL AUTO_INCREMENT,
  `CUSTOMER_ID` int(11) DEFAULT NULL,
  `JOB_DESC` text,
  `JOB_NEED_1` varchar(20) DEFAULT NULL,
  `JOB_NEED_2` varchar(20) DEFAULT NULL,
  `JOB_NEED_3` varchar(20) DEFAULT NULL,
  `JOB_START_TIME` datetime DEFAULT NULL,
  `JOB_DURATION` double DEFAULT '1',
  `JOB_STATUS` varchar(10) DEFAULT 'open',
  `JOB_ASSIGNED_ID` int(11) DEFAULT NULL,
  PRIMARY KEY (`JOB_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `JOB`
--

LOCK TABLES `JOB` WRITE;
/*!40000 ALTER TABLE `JOB` DISABLE KEYS */;
INSERT INTO `JOB` VALUES (1,1,'Change of leg dressing','Wound care',NULL,NULL,'2015-11-10 18:00:00',2.5,'open',NULL),(2,2,'General','Geriatrics',NULL,NULL,'2015-12-30 12:00:00',1,'open',NULL),(3,1,'General',NULL,NULL,NULL,'2015-10-10 18:00:00',1.5,'requested',6),(4,3,'Bathing and Turning','Geriatrics',NULL,NULL,'2015-11-28 12:00:00',1,'open',NULL),(5,4,'Ryles tube feeding',NULL,NULL,NULL,'2015-11-13 14:00:00',1,'open',NULL),(6,3,'Ryles tube feeding',NULL,NULL,NULL,'2015-11-15 18:00:00',1,'open',NULL),(7,2,'Bed sore dressing','Wound care','Geriatrics',NULL,'2015-12-18 18:00:00',2,'open',NULL),(8,2,'Turning and bathing','Geriatrics',NULL,NULL,'2015-10-16 14:00:00',3.5,'requested',4),(9,4,'General Care',NULL,NULL,NULL,'2015-12-18 21:00:00',9,'open',NULL),(10,3,'General',NULL,NULL,NULL,'2015-11-26 18:00:00',1,'requested',13),(11,5,'General',NULL,NULL,NULL,'2015-11-22 18:00:00',1,'open',NULL),(12,3,'Change of hand dressing',NULL,NULL,NULL,'2016-10-22 18:00:00',1,'open',NULL),(13,1,'General',NULL,NULL,NULL,'2015-12-22 19:30:00',1,'open',NULL),(14,1,'Ryles tube insertion',NULL,NULL,NULL,'2015-12-22 18:00:00',1,'open',NULL),(15,1,'Ryles tube insertion',NULL,NULL,NULL,'2015-09-22 18:00:00',1,'requested',4),(16,1,'Ryles tube insertion',NULL,NULL,NULL,'2015-12-08 17:30:00',1,'open',NULL),(17,1,'Bathing and turning',NULL,NULL,NULL,'2015-09-09 17:30:00',1,'confirmed',13),(18,1,'Ryles tube insertion and Feeding','Geriatrics',NULL,NULL,'2016-09-06 17:30:00',1,'open',NULL),(19,1,'Ryles tube insertion',NULL,NULL,NULL,'2015-10-22 18:00:00',1,'open',NULL),(20,1,'Ryles tube insertion',NULL,NULL,NULL,'2015-11-12 18:00:00',1,'open',NULL);
/*!40000 ALTER TABLE `JOB` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE /*!50017 DEFINER=`gcm_admin`@`%`*/ TRIGGER `JOB_UPDATE_TRG` AFTER UPDATE ON `JOB` FOR EACH ROW insert into JOB_AUDIT values (OLD.JOB_ID, OLD.JOB_DURATION,
                              OLD.JOB_STATUS,
                              NEW.JOB_STATUS,
                              OLD.JOB_ASSIGNED_ID,
                              NEW.JOB_ASSIGNED_ID,
                              CURRENT_TIMESTAMP);;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `JOB_AUDIT`
--

DROP TABLE IF EXISTS `JOB_AUDIT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `JOB_AUDIT` (
  `JOB_ID` int(11) NOT NULL,
  `JOB_DURATION` double DEFAULT NULL,
  `OLD_JOB_STATUS` varchar(10) DEFAULT NULL,
  `NEW_JOB_STATUS` varchar(10) DEFAULT NULL,
  `OLD_JOB_ASSIGNED_ID` int(11) DEFAULT NULL,
  `NEW_JOB_ASSIGNED_ID` int(11) DEFAULT NULL,
  `UPDATED_TIMESTAMP` timestamp NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `JOB_AUDIT`
--

LOCK TABLES `JOB_AUDIT` WRITE;
/*!40000 ALTER TABLE `JOB_AUDIT` DISABLE KEYS */;
INSERT INTO `JOB_AUDIT` VALUES (8,3.5,'open','requested',NULL,4,'2015-03-31 11:16:06');
/*!40000 ALTER TABLE `JOB_AUDIT` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gcm_users`
--

DROP TABLE IF EXISTS `gcm_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gcm_users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `gcm_regid` text,
  `name` varchar(50) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `phone` varchar(20) DEFAULT NULL,
  `NRIC` varchar(12) DEFAULT NULL,
  `date_of_birth` date DEFAULT NULL,
  `gender` varchar(6) DEFAULT NULL,
  `nurse_type` varchar(2) NOT NULL DEFAULT 'NN',
  `have_insurance` tinyint(1) NOT NULL DEFAULT '0',
  `verified` varchar(10) NOT NULL DEFAULT 'no',
  `balance` decimal(10,2) NOT NULL DEFAULT '0.00',
  `updatedon` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gcm_users`
--

LOCK TABLES `gcm_users` WRITE;
/*!40000 ALTER TABLE `gcm_users` DISABLE KEYS */;
INSERT INTO `gcm_users` VALUES (4,'APA91bHakYvY7DtSLuuXAGgC7WFgfULIJMGD3OWtunp8HZRSpQXZZOGTaucMcVERYkaeqMAtF_CXD5l_b5H-EheW9qmzqy8Lh8YKbaPCqDqWMfnmA1l1-0FrCQKFWTZDu26q5_pwxTMVZvEzE0nc5jXPH94Pnswr-Q','Nexus','alexlee188@gmail.com','98368954',NULL,'1955-11-27',NULL,'NN',0,'yes',14.00,'2015-03-31 11:16:23'),(5,'APA91bGdPzT_JZzEXdfTdEDwUwzOeOt5g3JD79GUvUiBjIkPE8-OebYwFWJX9OXw02pRFgrwiE3o3hQ8XBHiIVFnpawGJlUzlFvkr0euASR9lmJHzXS51JbwYLIgwmi2Avnrp_4e3m7tqzeCV6Tg4YZeZwq71ZTIog','S3','alexlee188@gmail.com','98368954',NULL,NULL,NULL,'NN',0,'yes',20.00,'2015-03-08 07:21:01'),(6,'APA91bGux4JL2aj958BnUlExxoJ1N-D1B7y5Z5EukvcLmdoqP0Lizgh9mzsB3rjmLy5HdVAFcRQIMj01ZuZQ5vgmyu6mnclXSJxiziojTk1oZvV-N3q5TOrF5OLWo781Qsw4RPR2ivhe4309cfMfHkBzqFKkBdzHYw','Nova 7','alexlee188@gmail.com','98368954',NULL,NULL,NULL,'NN',0,'yes',14.00,'2015-03-25 08:57:01'),(9,'APA91bGsIPiH5In59n1-XC9TOvcVb1Y0L7XMUmTVVtKXmlnh1NJVF7huMqy9KOaTI5vQgaCNjTYs1yPFX18fawFLJyEUA256FdrCPg_HLTwoWSxP74VrMkwVlufP7z8kzM6r-ZOTRQbHCDwA9XybDoR1M91x7iBXWw','note pro 12','babybaby@somewhere.org','',NULL,NULL,NULL,'NN',0,'yes',20.00,'2015-03-25 00:37:01'),(12,'APA91bFFB9lXCWR-DgLOu-E_9zftL4y1skU7HBwWq8OJC2JabUweUMphTxlauhLu5pJzt5XsCQN3AkB9u2FxdYhl1ON6rdx6N8K4ZW4xO7gLwyZzxuPiL7pxVi6AXN11NYMdvXzqW8OVRdg2bG-AEmEnixtbukczRA','Gayle','noname@somewhereelse.org','',NULL,NULL,NULL,'NN',0,'yes',20.00,'2015-03-08 07:21:18'),(13,'APA91bFzkKt-5mihF_0OyKFYz0NugISfdOHbVNNVMPHP2EtbdLRt4YZxh7CAAYmZrSnUyu28W1iI-pqDoWWzqAKyaG4amJlXQUSLcpAfBd6nUUnWotpdbQryskcZ-D5MIy3SEYNIHT2L5Ini8GtnKd-P2TgseXwKgQ','Redmi Note','alexlee188@gmail.com','98368954',NULL,NULL,NULL,'NN',0,'yes',13.00,'2015-03-29 03:36:49'),(15,'APA91bF4jeviq7T1P2bA58jKfQJxDUeSyNwFWR4rLMSOAVv8_JinzN2qgTNVXOEkPUt0pWenY6z6ZqsvzOy300sJDuiyIOkDVTY1i-3R39TwyE5HuWYTNDVx_TUFmrbgs_9PFjbjVLvup_Er3km0fV_KbbJlUr6cLQ','Wendy chua','chuachaiyee@gmail.com','97851440',NULL,NULL,NULL,'NN',0,'yes',184.00,'2015-03-17 05:24:14');
/*!40000 ALTER TABLE `gcm_users` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE /*!50017 DEFINER=`gcm_admin`@`%`*/ TRIGGER `gcm_users_UPDATE_TRG` AFTER UPDATE ON `gcm_users` FOR EACH ROW insert into gcm_users_AUDIT values (
    OLD.id, OLD.name, NEW.name, OLD.email, NEW.email,
    OLD.phone, NEW.phone, OLD.NRIC, NEW.NRIC, 
    OLD.date_of_birth, NEW.date_of_birth,
    OLD.gender, NEW.gender,
    OLD.nurse_type, NEW.nurse_type,
    OLD.have_insurance, NEW.have_insurance,
    OLD.verified, NEW.verified,
    OLD.gcm_regid, NEW.gcm_regid,
        OLD.balance, NEW.balance,
    CURRENT_TIMESTAMP);;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `gcm_users_AUDIT`
--

DROP TABLE IF EXISTS `gcm_users_AUDIT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gcm_users_AUDIT` (
  `id` int(11) NOT NULL,
  `old_name` varchar(50) DEFAULT NULL,
  `new_name` varchar(50) DEFAULT NULL,
  `old_email` varchar(255) DEFAULT NULL,
  `new_email` varchar(255) DEFAULT NULL,
  `old_phone` varchar(20) DEFAULT NULL,
  `new_phone` varchar(20) DEFAULT NULL,
  `old_NRIC` varchar(12) DEFAULT NULL,
  `new_NRIC` varchar(12) DEFAULT NULL,
  `old_date_of_birth` date DEFAULT NULL,
  `new_date_of_birth` date DEFAULT NULL,
  `old_gender` varchar(6) DEFAULT NULL,
  `new_gender` varchar(6) DEFAULT NULL,
  `old_nurse_type` varchar(2) DEFAULT NULL,
  `new_nurse_type` varchar(2) DEFAULT NULL,
  `old_have_insurance` tinyint(1) DEFAULT NULL,
  `new_have_insurance` tinyint(1) DEFAULT NULL,
  `old_verified` varchar(10) DEFAULT NULL,
  `new_verified` varchar(10) DEFAULT NULL,
  `old_gcm_regid` text,
  `new_gcm_regid` text,
  `old_balance` decimal(10,2) NOT NULL DEFAULT '0.00',
  `new_balance` decimal(10,2) NOT NULL DEFAULT '0.00',
  `UPDATED_TIMESTAMP` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gcm_users_AUDIT`
--

LOCK TABLES `gcm_users_AUDIT` WRITE;
/*!40000 ALTER TABLE `gcm_users_AUDIT` DISABLE KEYS */;
INSERT INTO `gcm_users_AUDIT` VALUES (4,'Nexus 7','Nexus 7','alexlee188@gmail.com','alexlee188@gmail.com','98368954','98368954',NULL,NULL,NULL,'1955-11-27',NULL,NULL,'NN','NN',0,0,'yes','yes','APA91bHakYvY7DtSLuuXAGgC7WFgfULIJMGD3OWtunp8HZRSpQXZZOGTaucMcVERYkaeqMAtF_CXD5l_b5H-EheW9qmzqy8Lh8YKbaPCqDqWMfnmA1l1-0FrCQKFWTZDu26q5_pwxTMVZvEzE0nc5jXPH94Pnswr-Q','APA91bHakYvY7DtSLuuXAGgC7WFgfULIJMGD3OWtunp8HZRSpQXZZOGTaucMcVERYkaeqMAtF_CXD5l_b5H-EheW9qmzqy8Lh8YKbaPCqDqWMfnmA1l1-0FrCQKFWTZDu26q5_pwxTMVZvEzE0nc5jXPH94Pnswr-Q',16.00,16.00,'2015-03-31 11:04:08'),(4,'Nexus 7','Nexus 7','alexlee188@gmail.com','alexlee188@gmail.com','98368954','98368954',NULL,NULL,'1955-11-27','1955-11-27',NULL,NULL,'NN','NN',0,0,'yes','yes','APA91bHakYvY7DtSLuuXAGgC7WFgfULIJMGD3OWtunp8HZRSpQXZZOGTaucMcVERYkaeqMAtF_CXD5l_b5H-EheW9qmzqy8Lh8YKbaPCqDqWMfnmA1l1-0FrCQKFWTZDu26q5_pwxTMVZvEzE0nc5jXPH94Pnswr-Q','APA91bHakYvY7DtSLuuXAGgC7WFgfULIJMGD3OWtunp8HZRSpQXZZOGTaucMcVERYkaeqMAtF_CXD5l_b5H-EheW9qmzqy8Lh8YKbaPCqDqWMfnmA1l1-0FrCQKFWTZDu26q5_pwxTMVZvEzE0nc5jXPH94Pnswr-Q',16.00,14.00,'2015-03-31 11:16:06'),(4,'Nexus 7','Nexus','alexlee188@gmail.com','alexlee188@gmail.com','98368954','98368954',NULL,NULL,'1955-11-27','1955-11-27',NULL,NULL,'NN','NN',0,0,'yes','yes','APA91bHakYvY7DtSLuuXAGgC7WFgfULIJMGD3OWtunp8HZRSpQXZZOGTaucMcVERYkaeqMAtF_CXD5l_b5H-EheW9qmzqy8Lh8YKbaPCqDqWMfnmA1l1-0FrCQKFWTZDu26q5_pwxTMVZvEzE0nc5jXPH94Pnswr-Q','APA91bHakYvY7DtSLuuXAGgC7WFgfULIJMGD3OWtunp8HZRSpQXZZOGTaucMcVERYkaeqMAtF_CXD5l_b5H-EheW9qmzqy8Lh8YKbaPCqDqWMfnmA1l1-0FrCQKFWTZDu26q5_pwxTMVZvEzE0nc5jXPH94Pnswr-Q',14.00,14.00,'2015-03-31 11:16:23');
/*!40000 ALTER TABLE `gcm_users_AUDIT` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-03-31 19:20:09
