SET @GOBJECT := 215424;
SET @QID4 := (SELECT questItem4 FROM gameobject_template WHERE entry = @GOBJECT);
SET @DATA0 := IF(@QID4 > 0, @QID4, 130698);
UPDATE gameobject_template SET questItem4=0, data0=@DATA0 WHERE entry=@GOBJECT;
