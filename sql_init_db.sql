
CREATE TABLE users(
		name text, salt text, password_hash text);

CREATE TABLE contexts(
		id bigint, name text, contents oid, parent_id bigint);

