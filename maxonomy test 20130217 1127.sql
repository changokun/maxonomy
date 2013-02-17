-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.1.53-community


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema test
--

CREATE DATABASE IF NOT EXISTS test;
USE test;

--
-- Definition of table `maxonomy`
--

DROP TABLE IF EXISTS `maxonomy`;
CREATE TABLE `maxonomy` (
  `item_class_id` tinyint(4) NOT NULL,
  `item_id` int(11) DEFAULT NULL,
  `tag_id` int(11) NOT NULL,
  KEY `by item` (`item_class_id`,`item_id`),
  KEY `by tag` (`tag_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `maxonomy`
--

/*!40000 ALTER TABLE `maxonomy` DISABLE KEYS */;
INSERT INTO `maxonomy` (`item_class_id`,`item_id`,`tag_id`) VALUES 
 (1,3,1),
 (1,4,3),
 (1,4,2),
 (1,5,3),
 (1,5,2),
 (1,6,3),
 (1,6,2),
 (1,7,3),
 (1,7,2),
 (1,8,3),
 (1,8,2),
 (1,9,1),
 (1,10,1),
 (1,11,10),
 (1,11,2),
 (1,12,10),
 (1,12,2),
 (1,13,10),
 (1,13,2),
 (1,14,10),
 (1,14,2),
 (1,15,10),
 (1,15,2),
 (1,16,1),
 (1,17,1);
/*!40000 ALTER TABLE `maxonomy` ENABLE KEYS */;


--
-- Definition of table `tag`
--

DROP TABLE IF EXISTS `tag`;
CREATE TABLE `tag` (
  `tag_id` int(11) NOT NULL AUTO_INCREMENT,
  `en` varchar(45) DEFAULT NULL,
  `es_LA` varchar(45) DEFAULT NULL,
  `en_GB` varchar(45) DEFAULT NULL,
  `ja` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`tag_id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tag`
--

/*!40000 ALTER TABLE `tag` DISABLE KEYS */;
INSERT INTO `tag` (`tag_id`,`en`,`es_LA`,`en_GB`,`ja`) VALUES 
 (1,'Continents',NULL,NULL,NULL),
 (2,'Countries',NULL,NULL,NULL),
 (3,'Africa',NULL,NULL,NULL),
 (4,'Djibouti',NULL,NULL,NULL),
 (5,'Republic of the Congo',NULL,NULL,NULL),
 (6,'Mozambique',NULL,NULL,NULL),
 (7,'Libya',NULL,NULL,NULL),
 (8,'Egypt',NULL,NULL,NULL),
 (9,'Asia',NULL,NULL,NULL),
 (10,'Europe',NULL,NULL,NULL),
 (11,'Great Britain',NULL,NULL,NULL),
 (12,'Poland',NULL,NULL,NULL),
 (13,'Spain',NULL,NULL,NULL),
 (14,'France',NULL,NULL,NULL),
 (15,'Andorra',NULL,NULL,NULL),
 (16,'North America',NULL,NULL,NULL),
 (17,'South America',NULL,NULL,NULL),
 (18,'temp',NULL,NULL,NULL);
/*!40000 ALTER TABLE `tag` ENABLE KEYS */;




/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
