-- World Trigger (59481) carried Turn On Blades (119315) on its template, so every World Trigger (Pandaria, Scholomance,
-- Siege of Orgrimmar) streamed flying blades nonstop. The aura belongs to a scripted encounter, not to the template.
UPDATE `creature_template_addon` SET `auras` = '' WHERE `entry` = 59481;
