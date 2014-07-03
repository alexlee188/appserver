-- MySQL dump 10.13  Distrib 5.5.38, for Linux (x86_64)
--
-- Host: localhost    Database: gcm
-- ------------------------------------------------------
-- Server version	5.5.38

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
-- Table structure for table `gcm_users`
--

DROP TABLE IF EXISTS `gcm_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gcm_users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `gcm_regid` text,
  `name` varchar(50) NOT NULL,
  `email` varchar(255) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gcm_users`
--

LOCK TABLES `gcm_users` WRITE;
/*!40000 ALTER TABLE `gcm_users` DISABLE KEYS */;
INSERT INTO `gcm_users` VALUES (1,'APA91bEnmreLQYNfeEusIXc1BUUQPa6qfFJJ1N1c9YPeaVRi1IsHecLWGK9Okvwlx145QEmixrMdTnSXtZTw-psCSPBUDXBEytQwveCaTxlhC5OFiYLAcTsj1txvhQ1GTPXsIy4HoYVLAJdjXW1mAiFZZhY0-q2nYA','xiaomi','noname@somewhere.org','2014-07-01 01:59:02'),(2,'APA91bGuwrip0MqxF-4JuzkEMNeRXGFpfzeQDuJP9ks4JcbmpEHdzSFp0yVOXttmeh-4fj4ih4ib5lyodqiVEmT2f2ymv_QSyC2nYCxC1xNajalup2F2GCo_blp0N__A5JbA57BcQRYnueykEBI_x1JLWb1DLFhIAQ','nexas 7','noname@somewhere.org','2014-07-01 02:15:02'),(4,'APA91bGjD6XhDUBr3NvcEr2Gp3iYc8-ybPpWWVPX4sy5eX9hOetVvEQZ4rV91g2hKVjX4nhZplB__31U4GizdJHrgzJ0SfkM4wDy4mgaT-zyZ1vu1z3fAKi6gOdTIjKx8lcL3EfVaXt6qymLkTaPvHtEwcmhA-4-hw','Note','noname@somewhere.org','2014-07-01 07:36:49'),(5,'APA91bEJLc1lcWPizz-JddRSXoV1rk3TUiUGxb2tNflbDKZ9wIuiENBWssuTycdwuYcTmAXXaC4efHhZZDPsSB2-gdA_ah8fwSNI_v6JFpulTe7s6tMKDNvJMW_3U8mgymXQBmVO6zOSHMGAPZwOHx16E45q1kGRDA','Samsung Tab','noname@somewhere.org','2014-07-01 08:59:50');
/*!40000 ALTER TABLE `gcm_users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-07-03 11:35:17
