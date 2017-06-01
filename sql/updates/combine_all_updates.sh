echo "Merging Updatefiles"
cat world/*.sql > world-updates.sql
cat characters/*.sql > characters-updates.sql
cat auth/*.sql > auth-updates.sql
