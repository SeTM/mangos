################### Warsong Gulch  ####################################
-- Focused and Brutal Assault should not be used outside WSG
DELETE FROM spell_area WHERE spell IN (46392,46393);
INSERT INTO spell_area VALUES
(46392,3321,0,0,0,0,0,2,0),
(46392,4571,0,0,0,0,0,2,0),
(46392,3277,0,0,0,0,0,2,0),
(46392,3320,0,0,0,0,0,2,0),
(46392,4572,0,0,0,0,0,2,0),
(46393,3321,0,0,0,0,0,2,0),
(46393,4571,0,0,0,0,0,2,0),
(46393,3277,0,0,0,0,0,2,0),
(46393,3320,0,0,0,0,0,2,0),
(46393,4572,0,0,0,0,0,2,0);

###################  general  ##########################################
-- Spirit healer prevent against death
-- unit_flags 4928
UPDATE creature_template SET unit_flags=unit_flags|2 WHERE entry IN (13117,13116);
