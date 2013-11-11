call run_stochastic_b3.bat  100 0
call run_stochastic_b3.bat  100 1
call run_stochastic_b3.bat  100 2
call run_stochastic_b3.bat  100 5
call run_stochastic_b3.bat  200 0
call run_stochastic_b3.bat  200 1
call run_stochastic_b3.bat  200 2
call run_stochastic_b3.bat  200 5
call run_stochastic_b3.bat  300 0
call run_stochastic_b3.bat  300 1
call run_stochastic_b3.bat  300 2
call run_stochastic_b3.bat  300 5

rem should run 2-2  not 1-1 below
mkdir user_study
cd user_study
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -r0 -I50 -b3 -kya_lee3:3:1-1:14-14:1-10
release\graph_based -r0 -I50 -b3 -krumi2:4:1-1:14-14:1-10
release\graph_based -r0 -I50 -b3 -kcrystal:5:1-1:14-14:1-10
release\graph_based -r0 -I50 -b3 -kteddy:6:1-1:14-14:1-10
release\graph_based -r0 -I50 -b3 -keva:7:1-1:14-14:1-10
release\graph_based -r0 -I50 -b3 -kanthony:8:1-1:14-14:1-10
release\graph_based -r0 -I50 -b3 -ksan:9:1-1:14-14:1-10
release\graph_based -r0 -I50 -b3 -ktyler:10:1-1:14-14:1-10
release\graph_based -r0 -I50 -b3 -kjames:11:1-1:14-14:1-10 
mv user_study user_study_error_50r0_b3_extra2