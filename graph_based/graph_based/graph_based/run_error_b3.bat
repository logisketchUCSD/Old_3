rem Error-driven

mkdir user_study
cd user_study
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -I%1 -r%2 -b3 -kya_lee3:3:2-2:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -krumi2:4:2-2:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -kcrystal:5:2-2:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -kteddy:6:2-2:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -keva:7:2-2:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -kanthony:8:2-2:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -ksan:9:2-2:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -ktyler:10:2-2:14-14:1-10
release\graph_based -I%1 -r%2 -b3 -kjames:11:2-2:14-14:1-10 
mv user_study user_study_error_%1r%2_b3
reminder "Done user_study_error_%1r%2_b3"