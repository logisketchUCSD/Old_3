rem run 1 0		% best 1, random 0
rem run 3 1		% best 3, random 1

mkdir user_study
cd user_study
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -b%1 -r%2 -kya_lee3:3:5-5:5-10:1-10
release\graph_based -b%1 -r%2 -krumi2:4:5-5:5-10:1-10
release\graph_based -b%1 -r%2 -kcrystal:5:5-5:5-10:1-10
release\graph_based -b%1 -r%2 -kteddy:6:5-5:5-10:1-10
release\graph_based -b%1 -r%2 -keva:7:5-5:5-10:1-10
release\graph_based -b%1 -r%2 -kanthony:8:5-5:5-10:1-10
release\graph_based -b%1 -r%2 -ksan:9:5-5:5-10:1-10
release\graph_based -b%1 -r%2 -ktyler:10:5-5:5-10:1-10
release\graph_based -b%1 -r%2 -kjames:11:5-5:5-10:1-10 
rem mv user_study user_study_b%1r%2
reminder "Done user_study_b%1r%2"