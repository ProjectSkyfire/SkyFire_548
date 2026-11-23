-- The Oculus: Varos Cloudstrider is made immune by his Centrifuge Shield, not by his template
UPDATE `creature_template` SET `unit_flags`=`unit_flags`&~0x00000300 WHERE `entry`=27447;
