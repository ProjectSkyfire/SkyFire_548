UPDATE creature_template SET ModLevel=1, minlevel=90+minlevel WHERE minlevel<0;
UPDATE creature_template SET ModLevel=1, maxlevel=90+maxlevel WHERE maxlevel<0;
