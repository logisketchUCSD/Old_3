call devin_error.bat 50  0
call devin_error.bat 50  1
call devin_error.bat 50  2
call devin_error.bat 50  5
call devin_error.bat 100 0
call devin_error.bat 100 1
call devin_error.bat 100 2
call devin_error.bat 100 5
call devin_error.bat 200 0
call devin_error.bat 200 1
call devin_error.bat 200 2
call devin_error.bat 200 5


mkdir user_study
cd user_study
mkdir 21
cd ..
release\graph_based -r0 -I50 -kdevinsmith:21:2-2:1-10:1-1
