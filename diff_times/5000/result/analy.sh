#!/bin/bash
EXAMPLES_DIR=`ls ../examples`

AL_DIR=`ls -F ../algorithm | grep "/$"`

TARGET="result_analysis"

if [ -d $TARGET_DIR ]
then
	echo "Removing directory $PWD/$TARGET"
fi

echo "Making and entrying directory $PWD/$TARGET"
mkdir $TARGET
cd $TARGET

for dir in $EXAMPLES_DIR
do
	echo "Making result directory"
	mkdir $dir
done
echo "Leaving directory $PWD"
cd ..

echo "Start analy result"

for dir in $AL_DIR
do
	for d in $EXAMPLES_DIR
	do

		OP_DIR="$PWD/../algorithm/${dir/\//}/result/${d/\//}"
		cd $OP_DIR

		files=`ls -tr ../../../../examples/$d`
		echo "Deleting $PWD/ChannelNum $PWD/ minEval $PWD/time"
		rm -rf ChannelNum minEval time
		echo "Creating file $PWD/channelNum "
		for f in $files
		do
			echo "cat $PWD/../../../../examples/${d/\//}/$f | wc -L >> channelNum"
			num=`cat $PWD/../../../../examples/${d/\//}/$f | wc -l`
			let num=$num-1
			echo $num>> ChannelNum
		done

		echo "awk '{print '\$4' >> "$OP_DIR/minEval"; print '\$7' >> "$OP_DIR/time";}' $OP_DIR/result"
		awk '{print $4 >> "minEval"; print $7 >> "time"; }' ./result
		cd -
	done
done

echo "Start collecting results"
for dir in $AL_DIR
do
	for d in $EXAMPLES_DIR
	do
		OP_DIR="$PWD/../algorithm/${dir/\//}/result/${d/\//}"
		echo "cp $OP_DIR/channelNum ./$TARGET/$d/channelNum"
		cp $OP_DIR/ChannelNum ./$TARGET/$d/channelNum
		echo "cp $OP_DIR/minEval ./$TARGET/$d/minEval_${dir/\//}"
		cp $OP_DIR/minEval ./$TARGET/$d/minEval_${dir/\//}
		echo "cp $OP_DIR/time ./$TARGET/$d/time_${dir/\//}"
		cp $OP_DIR/time ./$TARGET/$d/time_${dir/\//}
	done
done


echo "Analysis the collected files and creating the name.xls file"

DATA_DIR=`ls -F | grep "/$"`
cd $PWD/$DATA_DIR
echo "Entry directory " $PWD/DATA_DIR
RESULT_DIR=`ls -F | grep "/$"`

for d in $RESULT_DIR
do
        cd $d
        echo "Entry directory " $PWD/$d

        echo "Creating temp file .${d/\//}_binNum.tmp"
        T=`ls minEval* | tr '\n' ' '`
        TITLE="channelNum "$T
        echo $TITLE > .${d/\//}_minEval.tmp
        N=`ls minEval*`
        NUM="channelNum "$N
        echo $NUM | xargs paste >> .${d/\//}_minEval.tmp
        ../../produceXLS.py .${d/\//}_minEval.tmp minEval.xls


        echo "Creating temp file .${d/\//}_time.tmp"
        T=`ls time* | tr '\n' ' '`
        TITLE="channelNum "$T
        echo $TITLE  > .${d/\//}_time.tmp
        TI=`ls time*`
        TIME="channelNum "$TI
        echo $TIME | xargs paste >> .${d/\//}_time.tmp
        ../../produceXLS.py .${d/\//}_time.tmp time.xls

	echo "Creating file $PWD/minEvalTotal"
	cat .${d/\//}_minEval.tmp | tr '	' ' ' | cut -d ' ' -f 2- > minEvalTotal

	echo "Creating file $PWD/win_eval and $PWD/win_eval.xls"
	ARG2=`head -1 minEvalTotal | wc -w`
	../../stat/win minEvalTotal $ARG2 > win_eval
        ../../produceXLS.py win_eval win_eval.xls

	echo "Creating file $PWD/score and $PWD/score.xls"
	ARG2=`head -1 minEvalTotal | wc -w`
	../../stat/score minEvalTotal $ARG2 > score_eval
        ../../produceXLS.py score_eval score_eval.xls


	echo "Creating file $PWD/dif and dif.xls"
	ARG2=`head -1 minEvalTotal | wc -w`
	../../stat/dif minEvalTotal $ARG2 > dif_eval
        ../../produceXLS.py dif_eval dif_eval.xls

	echo "Creating file $PWD/minTime"
	cat .${d/\//}_time.tmp | tr '	' ' ' | cut -d ' ' -f 2- > minTime

	echo "Creating file $PWD/win_time and $PWD/win_time.xls"
	ARG2=`head -1 minTime | wc -w`
	../../stat/win minTime $ARG2 > win_time
        ../../produceXLS.py win_time win_time.xls

	echo "Creating file $PWD/score_time and $PWD/score_time.xls"
	ARG2=`head -1 minTime | wc -w`
	../../stat/score minTime $ARG2 > score_time
        ../../produceXLS.py score_time score_time.xls


	echo "Creating file $PWD/dif_time and dif_time.xls"
	ARG2=`head -1 minTime | wc -w`
	../../stat/dif minTime $ARG2 > dif_time
        ../../produceXLS.py dif_time dif_time.xls

        echo "Leaving directory " $PWD/$d
        cd ..
done

echo "Leaving directory " $PWD/$DATA_DIR
cd ..


echo "Leaving directory " $PWD/$DATA_DIR
cd ..

