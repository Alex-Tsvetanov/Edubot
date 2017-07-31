-- phpMyAdmin SQL Dump
-- version 4.7.0
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Jun 03, 2017 at 10:01 PM
-- Server version: 10.1.23-MariaDB
-- PHP Version: 7.1.5

USE `AIT`;

DROP TABLE IF EXISTS `Visits`;

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `AIT`
--

-- --------------------------------------------------------

--
-- Table structure for table `Visits`
--

CREATE TABLE `Visits` (
  `IdUser` int(11) NOT NULL,
  `Type` int(11) NOT NULL,
  `IdResource` int(11) NOT NULL,
  `ID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `Visits`
--

INSERT INTO `Visits` (`IdUser`, `Type`, `IdResource`, `ID`) VALUES
(1, 1, 1, 1),
(1, 1, 2, 2),
(2, 1, 1, 3);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `Visits`
--
ALTER TABLE `Visits`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `IdUser` (`IdUser`,`Type`,`IdResource`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `Visits`
--
ALTER TABLE `Visits`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
