call run_error_b3.bat 50  0
call run_error_b3.bat 50  1
call run_error_b3.bat 50  2
call run_error_b3.bat 50  5
call run_error_b3.bat 100 0
call run_error_b3.bat 100 1
call run_error_b3.bat 100 2
call run_error_b3.bat 100 5
call run_error_b3.bat 200 0
call run_error_b3.bat 200 1
call run_error_b3.bat 200 2
call run_error_b3.bat 200 5

rem Run the 1st one again to make sure it's not slower than the 1st one
mkdir user_study
cd user_study
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -r0 -I50 -b3 -kya_lee3:3:2-2:14-14:1-10
release\graph_based -r0 -I50 -b3 -krumi2:4:2-2:14-14:1-10
release\graph_based -r0 -I50 -b3 -kcrystal:5:2-2:14-14:1-10
release\graph_based -r0 -I50 -b3 -kteddy:6:2-2:14-14:1-10
release\graph_based -r0 -I50 -b3 -keva:7:2-2:14-14:1-10
release\graph_based -r0 -I50 -b3 -kanthony:8:2-2:14-14:1-10
release\graph_based -r0 -I50 -b3 -ksan:9:2-2:14-14:1-10
release\graph_based -r0 -I50 -b3 -ktyler:10:2-2:14-14:1-10
release\graph_based -r0 -I50 -b3 -kjames:11:2-2:14-14:1-10 
mv user_study user_study_error_50r0_b3_extra