#pragma once

#define TrainDataNum 60000
#define TestNum  10000
#define TrainNum 55000
#define ValiNum 5000
#define Row 28
#define Col 28
#define LabelType 10
#define BatchSize 5000

#define E 2.7
#define alpha 0.8
#define InputNum 1
#define OutputNum 1
#define HiddenNum 3
#define Depth HiddenNum + OutputNum

#define LinearActive(x)	\
	(x)
#define ExpActive(x)	\
	pow(E,x)
#define TanhActive(x)	\
	(pow(E,x) - pow(E,-x))/(pow(E,x) + pow(E,-x))
#define ReluActive(x)	\
	(((x) > (0)) ? (x) : (0))





