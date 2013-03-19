SELECT count(*) FROM information_schema.`COLUMNS`
WHERE table_name = '%1'
AND TABLE_SCHEMA = DATABASE()
