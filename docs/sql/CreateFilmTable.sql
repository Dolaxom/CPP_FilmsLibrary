CREATE TABLE film (
    id SERIAL PRIMARY KEY,
    title VARCHAR(150) NOT NULL,
    description TEXT,
    release_date DATE,
    rating INT,
    actors INT[]
);