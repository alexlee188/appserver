-- MySQL dump 10.13  Distrib 5.5.37, for debian-linux-gnu (x86_64)
--
-- Host: gcmdbinstance.cegfhjvyp8lf.ap-southeast-1.rds.amazonaws.com    Database: gcm
-- ------------------------------------------------------
-- Server version	5.6.17-log

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
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `JOB`
--

LOCK TABLES `JOB` WRITE;
/*!40000 ALTER TABLE `JOB` DISABLE KEYS */;
INSERT INTO `JOB` VALUES (1,1,'Change of leg dressing','Wound care',NULL,NULL,'2014-11-10 18:00:00',2.5,'assigned',7),(2,2,'General','Geriatrics',NULL,NULL,'2014-09-30 12:00:00',1,'assigned',8),(3,1,'General',NULL,NULL,NULL,'2014-10-10 18:00:00',1.5,'open',NULL),(4,3,'Bathing and Turning','Geriatrics',NULL,NULL,'2014-09-28 12:00:00',1,'open',NULL),(5,4,'Ryles tube feeding',NULL,NULL,NULL,'2014-11-13 14:00:00',1,'assigned',5),(6,3,'Ryles tube feeding',NULL,NULL,NULL,'2014-11-15 18:00:00',1,'assigned',4),(7,2,'Bed sore dressing','Wound care','Geriatrics',NULL,'2014-12-18 18:00:00',2,'open',NULL),(8,2,'Turning and bathing','Geriatrics',NULL,NULL,'2014-10-10 14:00:00',3.5,'assigned',2);
/*!40000 ALTER TABLE `JOB` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-07-13 13:30:53
