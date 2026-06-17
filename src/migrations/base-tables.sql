CREATE TABLE IF NOT EXISTS users
(
    id INT AUTO_INCREMENT,
    email VARCHAR(255) PRIMARY KEY,
    password VARCHAR(255) NOT NULL,
    name VARCHAR(255),
    lastname VARCHAR(255),
    created_at DATETIME,
    updated_at DATETIME,
    deleted_at DATETIME
);

CREATE TABLE IF NOT EXISTS restaurants
(
    id INT AUTO_INCREMENT,
    uuid VARCHAR(36) PRIMARY KEY,
    name VARCHAR(255),
    address VARCHAR(255),
    latitude DOUBLE NOT NULL,
    longitude DOUBLE NOT NULL,
    house_fias_id VARCHAR(255) NOT NULL UNIQUE,
    description TEXT,
    working_schedule TEXT,
    created_at DATETIME,
    updated_at DATETIME,
    deleted_at DATETIME
);

CREATE TABLE IF NOT EXISTS halls
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    uuid VARCHAR(36) NOT NULL UNIQUE,
    restaurant_uuid VARCHAR(36) NOT NULL,
    description TEXT,
    created_at DATETIME,
    updated_at DATETIME,
    deleted_at DATETIME,

    FOREIGN KEY (restaurant_uuid) REFERENCES restaurants (uuid)
);

CREATE TABLE IF NOT EXISTS tables
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    uuid VARCHAR(36) NOT NULL UNIQUE,
    name VARCHAR(255),
    hall_uuid VARCHAR(36) NOT NULL,
    guests_count INT NOT NULL,
    created_at DATETIME,
    updated_at DATETIME,
    deleted_at DATETIME,

    FOREIGN KEY (hall_uuid) REFERENCES halls (uuid)
);

CREATE TABLE IF NOT EXISTS users_booking
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_uuid VARCHAR(36) NOT NULL,
    restaurant_uuid VARCHAR(36) NOT NULL,
    hall_uuid VARCHAR(36) NOT NULL,
    table_uuid VARCHAR(36) NOT NULL,
    created_at DATETIME,
    updated_at DATETIME,
    deleted_at DATETIME,

    FOREIGN KEY (user_uuid) REFERENCES users (uuid),
    FOREIGN KEY (restaurant_uuid) REFERENCES restaurants (uuid),
    FOREIGN KEY (hall_uuid) REFERENCES halls (uuid),
    FOREIGN KEY (table_uuid) REFERENCES tables (uuid)
);