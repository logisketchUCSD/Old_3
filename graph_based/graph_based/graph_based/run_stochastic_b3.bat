rem Stochastic

mkdir user_study
cd user_study
rem mkdir 01 02 03 04 05 06 07 08 09 10 11
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -I%1 -r%2 -b3 -kya_lee3:3:1-1:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -krumi2:4:1-1:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -kcrystal:5:1-1:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -kteddy:6:1-1:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -keva:7:1-1:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -kanthony:8:1-1:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -ksan:9:1-1:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -ktyler:10:1-1:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -kjames:11:1-1:14-14:1-10 
mv user_study user_study_stochastic_%1r%2_b3
reminder "Done user_study_stochastic_%1r%2_b3"