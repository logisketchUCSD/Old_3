call run_stochastic.bat  100 0
call run_stochastic.bat  100 1
call run_stochastic.bat  100 2
call run_stochastic.bat  100 5
call run_stochastic.bat  200 0
call run_stochastic.bat  200 1
call run_stochastic.bat  200 2
call run_stochastic.bat  200 5
call run_stochastic.bat  300 0
call run_stochastic.bat  300 1
call run_stochastic.bat  300 2
call run_stochastic.bat  300 5

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
mv user_study user_study_error_50r0_extra1