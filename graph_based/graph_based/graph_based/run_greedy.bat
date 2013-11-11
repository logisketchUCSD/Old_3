rem Greedy

mkdir user_study
cd user_study
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -I%1 -r%2 -kya_lee3:3:3-3:14-14:1-10
release\graph_based -I%1 -r%2 -krumi2:4:3-3:14-14:1-10
release\graph_based -I%1 -r%2 -kcrystal:5:3-3:14-14:1-10
release\graph_based -I%1 -r%2 -kteddy:6:3-3:14-14:1-10
release\graph_based -I%1 -r%2 -keva:7:3-3:14-14:1-10
release\graph_based -I%1 -r%2 -kanthony:8:3-3:14-14:1-10
release\graph_based -I%1 -r%2 -ksan:9:3-3:14-14:1-10
release\graph_based -I%1 -r%2 -ktyler:10:3-3:14-14:1-10
release\graph_based -I%1 -r%2 -kjames:11:3-3:14-14:1-10 
mv user_study user_study_greedy_%1r%2
reminder "Done user_study_greedy_%1r%2"