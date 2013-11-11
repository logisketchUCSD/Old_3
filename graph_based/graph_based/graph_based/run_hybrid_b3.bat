rem Hybrid

mkdir user_study
cd user_study
mkdir 03 04 05 06 07 08 09 10 11
cd ..
release\graph_based -b3 -r%1 -kya_lee3:3:5-5:14-14:1-10
release\graph_based -b3 -r%1 -krumi2:4:5-5:14-14:1-10
release\graph_based -b3 -r%1 -kcrystal:5:5-5:14-14:1-10
release\graph_based -b3 -r%1 -kteddy:6:5-5:14-14:1-10
release\graph_based -b3 -r%1 -keva:7:5-5:14-14:1-10
release\graph_based -b3 -r%1 -kanthony:8:5-5:14-14:1-10
release\graph_based -b3 -r%1 -ksan:9:5-5:14-14:1-10
release\graph_based -b3 -r%1 -ktyler:10:5-5:14-14:1-10
release\graph_based -b3 -r%1 -kjames:11:5-5:14-14:1-10 
mv user_study user_study_hybrid_r%1_b3
reminder "Done user_study_hybrid_r%1_b3"