ocrWSegment * getWSegmentCV (ocrWRoutingGrid * grid,
			ocrVirtualConnector * i_pVirCon);

ocrWSegment * getWSegment (ocrWRoutingGrid * grid,
		   ocrNaturalInt x,
		   ocrNaturalInt y,
		   ocrNaturalInt z);

ocrRoutingDirection getWSegDirection (ocrRoutingParameters * i_param,
			   ocrWSegment * i_segment);

ocrWSegment * createWSegment (ocrNaturalInt offset,
			 ocrNaturalInt layer,
			 ocrNaturalInt p_min,
			 ocrNaturalInt p_max,
			 ocrNaturalInt signal_index);

void setWGrid (ocrWRoutingGrid * grid,
		ocrWSegment * segment,
		ocrNaturalInt x,
		ocrNaturalInt y,
		ocrNaturalInt z);


ocrNaturalInt findPathBetween2Points (ocrRoutingParameters * param,
				    ocrWRoutingGrid * grid,
				    ocrNaturalInt xsource,
				    ocrNaturalInt ysource,
				    ocrNaturalInt zsource,
				    ocrNaturalInt xtarget,
				    ocrNaturalInt ytarget,
				    ocrNaturalInt ztarget,
				    ocrNaturalInt signal_index,
				    ocrSignal * i_pSignal);


		




ocrRoutingParameters *
setParameters (ocrNaturalShort pitch_h,
               ocrNaturalShort pitch_v,
               ocrNaturalShort via_cost,
               ocrRoutingDirection dir_of_even_layers);

ocrWRoutingGrid *createWGrid (ocrNaturalInt size_h,
		   ocrNaturalInt size_v,
		   ocrNaturalInt nb_of_layers);

void initWGrid (ocrWRoutingGrid * i_pGrid,
		 ocrRoutingParameters * i_pParam);
void dumpIntExtConToPhFig (ocrRoutingDataBase * i_pDataBase,
				  ocrRoutingParameters * i_pParam,
				  phfig_list * i_pPhFig);
ocrNaturalInt dumpGridToPhFig (ocrRoutingParameters * i_pParam,
			  ocrWRoutingGrid * i_pGrid,
			  phfig_list * i_pPhFig);
void freeWGrid (ocrWRoutingGrid * i_pGrid);


