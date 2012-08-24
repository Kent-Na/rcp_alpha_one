--sql database init script for postgreSQL
--users

CREATE SEQUENCE users_id START 100;

CREATE TABLE IF NOT EXISTS users(
		id bigint UNIQUE DEFAULT nextval('users_id'),
		name text PRIMARY KEY, salt text, password_hash text);

ALTER SEQUENCE users_id OWNED BY users.id;

--contexts

CREATE SEQUENCE contexts_id START 100;

CREATE TABLE IF NOT EXISTS contexts(
		id bigint PRIMARY KEY DEFAULT nextval('contexts_id'),
		name text,
		contents oid DEFAULT 0,
		parent_id bigint);

ALTER SEQUENCE contexts_id OWNED BY contexts.id;
