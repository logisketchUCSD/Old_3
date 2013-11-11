call run_greedy.bat 1 0
call run_greedy.bat 1 1
call run_greedy.bat 1 2
call run_greedy.bat 1 5
call run_greedy.bat 2 0
call run_greedy.bat 2 1
call run_greedy.bat 2 2
call run_greedy.bat 2 5

rem Run the 1st one again to make sure it's not slower than the 1st one
mkdir user_study
cd user_study
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -r0 -I50 -kya_lee3:3:2-2:14-14:1-10
release\graph_based -r0 -I50 -krumi2:4:2-2:14-14:1-10
release\graph_based -r0 -I50 -kcrystal:5:2-2:14-14:1-10
release\graph_based -r0 -I50 -kteddy:6:2-2:14-14:1-10
release\graph_based -r0 -I50 -keva:7:2-2:14-14:1-10
release\graph_based -r0 -I50 -kanthony:8:2-2:14-14:1-10
release\graph_based -r0 -I50 -ksan:9:2-2:14-14:1-10
release\graph_based -r0 -I50 -ktyler:10:2-2:14-14:1-10
release\graph_based -r0 -I50 -kjames:11:2-2:14-14:1-10 
mv user_study user_study_error_50r0_extra3