-- =====================
-- Database: movieticketdb
-- =====================
DROP DATABASE IF EXISTS movieticketdb;
CREATE DATABASE movieticketdb;
USE movieticketdb;

-- =====================
-- Table: movie_tb
-- =====================
CREATE TABLE movie_tb (
  movie_id INT AUTO_INCREMENT PRIMARY KEY,
  title VARCHAR(100) NOT NULL,
  m_genre VARCHAR(50) NOT NULL,
  duration_minutes INT NOT NULL,
  description TEXT
);

INSERT INTO movie_tb (title, m_genre, duration_minutes, description) VALUES
('Venom 2', 'Fantasy', 130, 'Venom returns to face Carnage'),
('Spiderman 2', 'Action', 145, 'Peter Parker saves the city again'),
('Inception', 'Sci-Fi', 150, 'Dream within a dream');

-- =====================
-- Table: cinema_tb
-- =====================
CREATE TABLE cinema_tb (
  cinema_id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(100) NOT NULL,
  address VARCHAR(200) NOT NULL
);

INSERT INTO cinema_tb (name, address) VALUES
('CGV Vincom', '54 Le Van Viet, Thu Duc'),
('Lotte Cinema', '50 Nguyen Van Linh, District 7');

-- =====================
-- Table: hall_tb
-- =====================
CREATE TABLE hall_tb (
  hall_id INT AUTO_INCREMENT PRIMARY KEY,
  cinema_id INT NOT NULL,
  hall_name VARCHAR(50) NOT NULL,
  seat_count INT NOT NULL,	
  FOREIGN KEY (cinema_id) REFERENCES cinema_tb(cinema_id)
);

INSERT INTO hall_tb (cinema_id, hall_name, seat_count) VALUES
(1, 'Hall A', 100),
(1, 'Hall B', 120),
(2, 'Hall 1', 150);

-- =====================
-- Table: screening_tb
-- =====================
CREATE TABLE screening_tb (
  screening_id INT AUTO_INCREMENT PRIMARY KEY,
  movie_id INT NOT NULL,
  cinema_id INT NOT NULL,
  hall_id INT NOT NULL,
  show_date DATE NOT NULL,
  show_time VARCHAR(20) NOT NULL,
  ticket_price INT NOT NULL,
  available_seats INT NOT NULL,
  FOREIGN KEY (movie_id) REFERENCES movie_tb(movie_id),
  FOREIGN KEY (cinema_id) REFERENCES cinema_tb(cinema_id),
  FOREIGN KEY (hall_id) REFERENCES hall_tb(hall_id)
);

INSERT INTO screening_tb (movie_id, cinema_id, hall_id, show_date, show_time, ticket_price, available_seats) VALUES
(1, 1, 1, '2025-10-24', '09:00', 90000, 100),
(2, 1, 2, '2025-10-24', '19:30', 120000, 120),
(3, 2, 3, '2025-10-25', '21:00', 100000, 150);

-- =====================
-- Table: customer_tb
-- =====================
CREATE TABLE customer_tb (
  customer_id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(100) NOT NULL,
  phone VARCHAR(20) NOT NULL,
  email VARCHAR(100),
  username VARCHAR(50) UNIQUE,
  password VARCHAR(100)
);

INSERT INTO customer_tb (name, phone, email) VALUES
('Adones', '0905123456', 'adones@example.com'),
('Hieu', '0905111111', 'hieu@example.com');

-- Cập nhật tài khoản có username/password
UPDATE customer_tb 
SET username = 'adones', password = 'adones123' 
WHERE customer_id = 1;

INSERT INTO customer_tb (name, phone, email, username, password)
VALUES ('NewUser', '0905000000', 'newuser@example.com', 'newuser', 'newpass');

-- =====================
-- Table: admin_tb
-- =====================
CREATE TABLE admin_tb (
  admin_id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(50) UNIQUE NOT NULL,
  password VARCHAR(100) NOT NULL
);

INSERT INTO admin_tb (username, password)
VALUES ('admin', 'admin123');

-- =====================
-- Table: booking_tb
-- =====================
CREATE TABLE booking_tb (
  booking_id INT AUTO_INCREMENT PRIMARY KEY,
  customer_id INT NOT NULL,
  screening_id INT NOT NULL,
  seats VARCHAR(50) NOT NULL,
  seat_count INT NOT NULL,
  total_price DOUBLE NOT NULL,
  status VARCHAR(20) DEFAULT 'PENDING',
  qr_code VARCHAR(100),
  booked_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (customer_id) REFERENCES customer_tb(customer_id),
  FOREIGN KEY (screening_id) REFERENCES screening_tb(screening_id)
);
select * from customer_tb