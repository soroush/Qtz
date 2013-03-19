SELECT t.name AS table_name, i.rows
FROM sys.tables AS t INNER JOIN
sys.sysindexes AS i ON t.object_id = i.id AND i.indid < 2