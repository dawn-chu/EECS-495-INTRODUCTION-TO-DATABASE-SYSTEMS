DELIMITER //

CREATE PROCEDURE enrollclass
(IN studentID char(20),
 IN courseyear real,
 IN coursesemester char(20))
Begin
SELECT distinct uo.UoSCode
From uosoffering uo
where     uo.year=courseyear
	  and uo.semester=coursesemester
      and uo.MaxEnrollment>uo.Enrollment 
      and ((uo.UoSCode in (select r.UoSCode
							from requires r
							where r.PrereqUoSCode not in
											(select t.UoSCode
											 from transcript t
											 where t.StudId=studentID and t.grade is NULL ) ))or (uo.UoSCode not in (select r.UoSCode from requires r)))
	  and uo.uoscode not in ( select r.UoSCode 
                              from requires r,requires s
                              where r.uoscode=s.uoscode and r.prerequoscode<>s.prerequoscode 
                                    and r.prerequoscode in ( select t.UoSCode from transcript t
							  where t.studid=studentID and t.grade is null)
		                            or r.prerequoscode not in ( select t.UoSCode from transcript t
							        where t.studid=studentID ) )
	  and uo.UoSCode not in ( select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.grade is null)
	and uo.UoSCode  not in ( select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.year=courseyear and t.semester=coursesemester and t.grade is not null);
                              
END//	

DELIMITER ;

DELIMITER //

CREATE PROCEDURE cannotenroll
(IN studentID char(20),
 IN courseyear real,
 IN coursesemester char(20))
Begin
SELECT distinct uo.UoSCode
From uosoffering uo
where     uo.year=courseyear
	  and uo.semester=coursesemester
      and uo.MaxEnrollment>uo.Enrollment 
      and (uo.UoSCode  in    (select r.UoSCode
							from requires r
							where r.PrereqUoSCode  in
											(select t.UoSCode
											 from transcript t
											 where t.StudId=studentID and t.grade is NULL ))
	  or uo.uoscode in     ( select r.UoSCode 
                              from requires r,requires s
                              where r.uoscode=s.uoscode and r.prerequoscode<>s.prerequoscode 
                                    and r.prerequoscode in ( select t.UoSCode from transcript t
							  where t.studid=studentID and t.grade is null)
		                            or r.prerequoscode not in ( select t.UoSCode from transcript t
							        where t.studid=studentID ) )
	  or uo.UoSCode in     ( select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.grade is null) )
		or uo.UoSCode  in ( select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.year=courseyear and t.semester=coursesemester and t.grade is not null);
END//	

DELIMITER ;






DELIMITER //

CREATE PROCEDURE whetherrightclass
(IN chosecourse char(20),
 IN studentID char(20),
 IN courseyear real,
 IN coursesemester char(20))
Begin
SELECT distinct uo.UoSCode
From uosoffering uo
where     uo.year=courseyear
	  and uo.semester=coursesemester
      and uo.MaxEnrollment>uo.Enrollment 
      and ((uo.UoSCode in (select r.UoSCode
							from requires r
							where r.PrereqUoSCode not in
											(select t.UoSCode
											 from transcript t
											 where t.StudId=studentID and t.grade is NULL ) ))or (uo.UoSCode not in (select r.UoSCode from requires r)))
	  and uo.uoscode not in ( select r.UoSCode 
                              from requires r,requires s
                              where r.uoscode=s.uoscode and r.prerequoscode<>s.prerequoscode 
                                    and r.prerequoscode in ( select t.UoSCode from transcript t
							  where t.studid=studentID and t.grade is null)
		                            or r.prerequoscode not in ( select t.UoSCode from transcript t
							        where t.studid=studentID ) )
	  and uo.UoSCode not in ( select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.grade is null)
		and uo.UoSCode  not in ( select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.year=courseyear and t.semester=coursesemester and t.grade is not null)
	  and uo.UoSCode=chosecourse;
END//	

DELIMITER ;

DELIMITER //

CREATE PROCEDURE needpreclass
(IN chosecourse char(20),
 IN studentID char(20),
 IN courseyear real,
 IN coursesemester char(20))
Begin
SELECT distinct uo.UoSCode
From uosoffering uo
where     uo.year=courseyear
	  and uo.semester=coursesemester
      and uo.MaxEnrollment>uo.Enrollment 
      and (uo.UoSCode  in    (select r.UoSCode
							from requires r
							where r.PrereqUoSCode  in
											(select t.UoSCode
											 from transcript t
											 where t.StudId=studentID and t.grade is NULL ))
	  or uo.uoscode in     ( select r.UoSCode 
                              from requires r,requires s
                              where r.uoscode=s.uoscode and r.prerequoscode<>s.prerequoscode 
                                    and r.prerequoscode in ( select t.UoSCode from transcript t
							  where t.studid=studentID and t.grade is null)
		                            or r.prerequoscode not in ( select t.UoSCode from transcript t
							        where t.studid=studentID ) )
	  or uo.UoSCode in     ( select t.UoSCode   
                              from  transcript t
                              where t.StudId=studentID and t.grade is null) )
		
	  and uo.UoSCode=chosecourse;
END//	

DELIMITER ;


DELIMITER //

CREATE PROCEDURE preclass
(IN chosecourse char(20))
Begin
SELECT r.PrereqUoSCode
FROM requires r
WHERE r.UoSCode=chosecourse ;
END//	

DELIMITER ;

DELIMITER //

CREATE PROCEDURE addclass
( IN studentID char(20),
  IN coursename char(20),
  IN courseyear real,
  IN coursesemester char(20))

BEGIN



START TRANSACTION;

Insert INTO transcript
values(studentID,coursename,coursesemester,courseyear,null);
update uosoffering
set uosoffering.Enrollment=uosoffering.Enrollment+1
where uosoffering.Uoscode=coursename and uosoffering.Semester=coursesemester and uosoffering.Year=courseyear;

COMMIT;
END//	

DELIMITER ;