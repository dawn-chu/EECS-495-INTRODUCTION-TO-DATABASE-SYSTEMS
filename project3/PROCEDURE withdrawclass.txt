DELIMITER //

CREATE PROCEDURE withdrawclass
(IN studentID char(20),
 IN courseyear real,
 IN coursesemester char(20))
Begin
SELECT distinct uo.UoSCode
From uosoffering uo
where     uo.year=courseyear
	  and uo.semester=coursesemester
		and uo.UoSCode in(select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.grade is null and t.Semester=coursesemester and t.year=courseyear);
END//	

DELIMITER ;

DELIMITER //

CREATE PROCEDURE whether_right_withdraw
(IN chosecourse char(20),
 IN studentID char(20),
 IN courseyear real,
 IN coursesemester char(20))
Begin
SELECT distinct uo.UoSCode
From uosoffering uo
where     uo.year=courseyear
	    and uo.semester=coursesemester
        and uo.UoSCode in(select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.grade is null)
		and uo.UoSCode=chosecourse ;
END//	

DELIMITER ;

DELIMITER //

CREATE PROCEDURE decreaseclass
( IN studentID char(20),
  IN coursename char(20),
  IN courseyear real,
  IN coursesemester char(20))
BEGIN

DECLARE exit handler for sqlexception
  BEGIN
    -- ERROR
  ROLLBACK;
END;

DECLARE exit handler for sqlwarning
 BEGIN
    -- WARNING
 ROLLBACK;
END;

START TRANSACTION;

DELETE FROM transcript 
WHERE transcript.StudId=studentID and transcript.UoSCode=coursename 
and transcript.Grade is null and transcript.year=courseyear and transcript.Semester=coursesemester;

update uosoffering
set uosoffering.Enrollment=uosoffering.Enrollment-1
where uosoffering.Uoscode=coursename and uosoffering.Semester=coursesemester and uosoffering.Year=courseyear;

COMMIT;
END//	

DELIMITER ;



