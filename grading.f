PROGRAM  LetterGrade
   IMPLICIT   NONE

   REAL              :: mark
   CHARACTER(LEN=2)  :: Grade

   READ(*,*)  mark

   SELECT CASE (NINT(mark))     ! round Average before use
      CASE (:32)                   ! <= 59 -------------> F
         Grade = 'F '
      CASE (33:39)                 ! >= 60 and <= 64 ---> D
         Grade = 'D '
      CASE (40:49)                 ! >= 65 and <= 69 ---> CD
         Grade = 'C '
      CASE (50:59)                 ! >= 70 and <= 74 ---> C
         Grade = 'B '
      CASE (60:69)                 ! >= 75 and <= 79 ---> BC
         Grade = 'A-'
      CASE (70:79)                 ! >= 80 and <= 84 ---> B
         Grade = 'A '
      CASE (80:100)                 ! >= 84 and <= 89 ---> AB
         Grade = 'A+'
   END SELECT
   
   WRITE(*,*)  'Letter Grade  : ', Grade

END PROGRAM  LetterGrade
