SET @MFFEVENT := 1;    -- Midsummer Fire Festival
SET @DWQUEST := 32497; -- Honor the Flame (Dread Wastes)
SET @JFQUEST := 32498; -- Honor the Flame (Jade Forest)
SET @KWQUEST := 32499; -- Honor the Flame (Krasarang Wilds)
SET @KLQUEST := 32500; -- Honor the Flame (Kun-Lai Summit)
SET @TSQUEST := 32501; -- Honor the Flame (Townlong Steppes)
SET @VWQUEST := 32502; -- Honor the Flame (Valley of the Four Winds)

DELETE FROM `game_event_seasonal_questrelation` WHERE `questId` IN (@DWQUEST, @JFQUEST, @KWQUEST, @KLQUEST, @TSQUEST, @VWQUEST) AND `eventEntry`=@MFFEVENT;
INSERT INTO `game_event_seasonal_questrelation` (`questId`, `eventEntry`) VALUES
(@DWQUEST, @MFFEVENT),
(@JFQUEST, @MFFEVENT),
(@KWQUEST, @MFFEVENT),
(@KLQUEST, @MFFEVENT),
(@TSQUEST, @MFFEVENT),
(@VWQUEST, @MFFEVENT);
