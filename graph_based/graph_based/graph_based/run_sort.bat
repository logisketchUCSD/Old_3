rem Sort

mkdir user_study
cd user_study
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -r%1 -kya_lee3:3:4-4:14-14:1-10
release\graph_based -r%1 -krumi2:4:4-4:14-14:1-10
release\graph_based -r%1 -kcrystal:5:4-4:14-14:1-10
release\graph_based -r%1 -kteddy:6:4-4:14-14:1-10
release\graph_based -r%1 -keva:7:4-4:14-14:1-10
release\graph_based -r%1 -kanthony:8:4-4:14-14:1-10
release\graph_based -r%1 -ksan:9:4-4:14-14:1-10
release\graph_based -r%1 -ktyler:10:4-4:14-14:1-10
release\graph_based -r%1 -kjames:11:4-4:14-14:1-10 
mv user_study user_study_sort_r%1
reminder "Done user_study_sort_r%1"