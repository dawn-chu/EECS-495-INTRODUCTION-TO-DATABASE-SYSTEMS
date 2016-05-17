DROP TRIGGER if exists MAXEnrollment;
drop TABLE if exists Warning;

DELIMITER //
CREATE TABLE Warning (signal1 int(1)); //
DELIMITER ;

DELIMITER //
CREATE TRIGGER MAXEnrollment
    BEFORE UPDATE ON uosoffering
    FOR EACH ROW
    BEGIN
	if new.Enrollment<(0.5*NEW.MAXEnrollment)
	then	 
        insert into warning
        values(1);	
        end if;
	if new.Enrollment>=(0.5*NEW.MAXEnrollment)
	then 
		delete from warning; 
    end if;
    end ;
    //
DELIMITER ;