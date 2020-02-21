UPDATE creature_template SET modlevel=0 WHERE minlevel > 0 AND minlevel < 90 AND maxlevel > 0 AND maxlevel < 90;
UPDATE creature_template SET modlevel=1 WHERE minlevel < 1 AND minlevel > 90 AND maxlevel < 1 AND maxlevel > 90;
