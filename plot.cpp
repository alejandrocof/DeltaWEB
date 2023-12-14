#include "plot.h"

//http://ayapin-film.sakura.ne.jp/Gnuplot/Docs/ps_guide.pdf
Plot::Plot(){
    logscalexy=false;
    logscalex=false;
    logscaley=false;
    multipleDeployments=false;
    ymin=0.0;
    ymax=0.0;
    xmin=0.0;
    xmax=0.0;
    dmax=0.0;
    //gp = Gnuplot();

    //*gp<< "set term postscript eps enhanced color font 'Helvetica,12'"<<endl;
    //*gp<< "set terminal wxt enhanced font 'Helvetica,12' persist"<<endl;
    gp<< "set terminal svg size 800,600"<<endl;


    gp << "set encoding iso_8859_1"<<endl;
    gp.precision(4);
    gp.width(7);
    gp <<std::fixed;
    gp << "set grid xtics ytics mxtics mytics lc rgb 'black' lw 0.5 lt 1, lc rgb 'gray50' lt 1 lw 0.2"<<endl;

    gp<< "set style line 1 lc rgb '#16469D' lt 1 lw 1.0 ps 0.5 pt 7 # dark blue"<<endl;
    gp<< "set style line 2 lc rgb '#BD202D' lt 1 lw 1.0 ps 0.5 pt 7 # red"<<endl;
    gp<< "set style line 3 lc rgb '#000000' lt 1 lw 1.0 ps 0.5 pt 7 # black"<<endl;
    gp<< "set style line 4 lc rgb '#00A14B' lt 1 lw 0.8 ps 0.5 pt 7 # green"<<endl;
    gp<< "set style line 5 lc rgb '#4B96D1' lt 1 lw 0.8 ps 0.5 pt 7 # light blue"<<endl;
    gp<< "set style line 6 lc rgb '#F16521' lt 1 lw 0.8 ps 0.5 pt 7 # orange"<<endl;
    gp<< "set style line 7 lc rgb '#9F6EAF' lt 1 lw 0.8 ps 0.5 pt 7 # light purple"<<endl;
    gp<< "set style line 8 lc rgb '#4C83E6' lt 2 lw 1.0 ps 0.5 pt 7 # light blue"<<endl;
    gp<< "set style line 9 lc rgb '#E1515D' lt 2 lw 1.0 ps 0.5 pt 7 # red"<<endl;
    gp<< "set style line 10 lc rgb '#999999' lt 2 lw 1.0 ps 0.5 pt 7 # black"<<endl;
    gp<< "set style line 11 lc rgb '#00A14B' lt 2 lw 0.8 ps 0.5 pt 7 # green"<<endl;
    gp<< "set style line 12 lc rgb '#4B96D1' lt 2 lw 0.8 ps 0.5 pt 7 # light blue"<<endl;
    gp<< "set style line 13 lc rgb '#F16521' lt 2 lw 0.8 ps 0.5 pt 7 # orange"<<endl;
    gp<< "set style line 14 lc rgb '#9F6EAF' lt 2 lw 0.8 ps 0.5 pt 7 # light purple"<<endl;
}

void Plot::push_dat(Data2D &d){
    _dat.push_back(d);
}

void Plot::push_dat( Data2D &d1, Data2D &d2){
    _dat.push_back(d1);
    _dat2.push_back(d2);
    dobleData=true;
}

void Plot::draw(string nameFileSVG){
    //cout<<"set output '"<<name<<".eps'"<<endl;
    gp<< "set output '"<<nameFileSVG<<endl;

    if(mxtics!=-1){
        gp << "set mxtics "<<mxtics<<endl;
    }
    if(mytics!=-1){
        gp << "set mytics "<<mytics<<endl;
    }

    if(logscalexy){
        gp << "set logscale xy"<<endl;
        gp << "set format x '10^{%L}'"<<endl;
        gp<<"set xtics nomirror border offset 0.2,0.2"<<endl;
        gp << "set format y '10^{%L}'"<<endl;
        gp<<"set ytics nomirror border offset 0.7,0.2"<<endl;
    }
    if(logscalex){
        gp << "set logscale x"<<endl;
        gp << "set format x '10^{%L}'"<<endl;
    }
    if(logscaley){
        gp << "set logscale y"<<endl;
        gp << "set format y '10^{%L}'"<<endl;
    }

    if(dmax!=0.0)
        gp << "set label 1 'max="<<std::fixed<<std::setprecision(2)<<dmax<<units<<"' at graph 0.98, graph 0.8 right"<<endl;
    gp <<"set clip two"<<endl;//https://stackoverflow.com/questions/30274786/gnuplot-wxt-not-working-anymore

    if(multipleDeployments){

        gp<< "set offsets"<<endl;
        gp<< "set autoscale fix"<<endl;
        gp<< "set size 1,1"<<endl;
        gp<< "set multiplot layout 3,1 title '"<<title<<"' font 'Helvetica,12'"<<endl;
        gp<< "set format x ''"<<endl;

        for(unsigned int i=0;i<_dat.size();i++){

            double msuperior=0.05;
            double minferior=0.05;
            double mizquierdo=0.08;
            double mderecho=0.02;
            double alto=(1.0-msuperior-minferior)/_dat.size();
            double escala=0.9;
            int indice=_dat.size()-1-i;
            gp << "set tmargin at screen "<< minferior+alto*(indice+1)<<endl;
            gp << "set bmargin at screen "<< minferior+alto*(indice+1-escala)<<endl;
            gp << "set lmargin at screen "<<mizquierdo<<endl;
            gp << "set rmargin at screen "<<1.0-mderecho<<endl;
            //gp << "set xrange ["<<0<<":"<<ceil( vreg[iReg].dt[j]*vreg[iReg].a.size() ) <<"]"<<endl;
            //std::cout<< "set xrange ["<<0<<":"<<ceil( vreg[iReg].dt[j]*vreg[iReg].a.size() ) <<"]"<<endl;
            //gp << "set yrange ["<<-amax<<":"<<amax<<"]"<<endl;
            if(xmin!=0.0 || xmax!=0.0) gp <<std::fixed<<std::setprecision(20)<< "set xrange ["<<xmin<<":"<<xmax<<"]"<<endl;
            if(ymin!=0.0 || ymax!=0.0) gp <<std::fixed<<std::setprecision(20)<< "set yrange ["<<ymin<<":"<<ymax<<"]"<<endl;
            //gp << "set yrange ["<<-Amax[j]<<":"<<Amax[j]<<"]"<<endl;
            gp << "set grid x y"<<endl;
            gp << "set grid xtics ytics lc rgb '#5F5F5F' lt -1 lw 0.2"<<endl;
            //*gp << "set yrange font 'Helvetica,10"<<endl;




            //gp << "set label 1 'Amax="<<std::fixed<<std::setprecision(2)<<fabs(vreg[iReg].Ramax[j])<<"cm/s^{_2}' at graph 0.82, graph 0.8"<<endl;
            //*gp << "set label 1 'max="<<std::fixed<<std::setprecision(2)<<_dat[i].max<<units<<"' at graph 0.98, graph 0.8 right"<<endl;
            if(dobleData){
                gp << "set label 1 'max="<<std::fixed<<std::setprecision(2)<<_dat[i].max<<units<<"' at graph 0.84, graph 0.2 left"<<endl;
                gp << "set label 2 'max="<<std::fixed<<std::setprecision(2)<<_dat2[i].max<<units<<"' at graph 0.84, graph 0.1 left"<<endl;
            }
            else{
              gp << "set label 1 'max="<<std::fixed<<std::setprecision(2)<<_dat[i].max<<units<<"' at graph 0.98, graph 0.8 right"<<endl;
            }

            gp<<"unset xlabel"<<endl;
            gp<<"unset ylabel"<<endl;

            gp<<"set ytics nomirror border offset 0.8,0.2"<<endl;

            if(i==_dat.size()-1){
                //*gp<<"set xtics nomirror"<<endl;
                gp<<"set xtics nomirror border offset 0,0.5"<<endl;
                //set xtics border offset 0,0.5 -5,1,5
                gp<<"set format x '%g'"<<endl;
                gp << "set xlabel '"<< xlabel <<"' offset 0,0.5 font 'Helvetica,12'"<<endl;
            }

            if(i==(_dat.size()-1)/2)
                gp << "set ylabel '"<< ylabel <<"' offset 1.2,0 font 'Helvetica,12'"<<endl;


            if(dobleData){
                std::vector< double > t1(_dat[i].y.size());
                std::vector< double > t2(_dat2[i].y.size());
                //cout<<"draw"<<endl;
                for(unsigned int j=0;j<t1.size();j++){
                    t1[j]=_dat[i].ti+_dat[i].dt*j;

                }
                for(unsigned int j=0;j<t2.size();j++){
                    t2[j]=_dat2[i].ti+_dat2[i].dt*j;

                }
                //std::cout<<t.size()<<" max ("<<t_max<<", "<<max<<") -> "<<1.0/t_max<<std::endl;
                gp <<"plot "<<gp.file1d( std::make_pair(t1,_dat[i].y) )<< " with lines ls "<<i+1<<" title '"<<_dat[i].name_raw <<" : "<< _dat[i].component <<"',";
                gp <<gp.file1d( std::make_pair(t2,_dat2[i].y) )<< " with lines ls "<<i+8<<" title '"<<_dat2[i].name_raw <<" : "<<_dat2[i].component <<"'"<<std::endl;
                //*gp <<"show style line"<<std::endl;
                //*gp <<"plot "<<gp->file1d( std::make_pair(t1,_dat[i].y) )<< " lt 1 lc 3 title '"<<_dat[i].component <<"',";
                //*gp <<gp->file1d( std::make_pair(t2,_dat2[i].y) )<< " lt 3 lc 3 title '"<<_dat2[i].component <<"'"<<std::endl;
            }
            else{
                std::vector< double > t(_dat[i].y.size());
                //cout<<"draw"<<endl;
                for(unsigned int j=0;j<t.size();j++){
                    t[j]=_dat[i].ti+_dat[i].dt*j;

                }
                //std::cout<<t.size()<<" max ("<<t_max<<", "<<max<<") -> "<<1.0/t_max<<std::endl;
                gp <<"plot "<<gp.file1d( std::make_pair(t,_dat[i].y) )<< " with lines ls "<<i+1<<" title '"<<_dat[i].name_raw <<" : "<<_dat[i].component <<"'"<<std::endl;
            }
            gp << "unset label"<<endl;
        }
    }
    else{
        if(xmin!=0.0 || xmax!=0.0) gp <<std::fixed<<std::setprecision(20)<< "set xrange ["<<xmin<<":"<<xmax<<"]"<<endl;
        if(ymin!=0.0 || ymax!=0.0) gp <<std::fixed<<std::setprecision(20)<< "set yrange ["<<ymin<<":"<<ymax<<"]"<<endl;
        gp << "set title '"<<title<<"' font 'Helvetica,16'"<<endl;
        gp << "set xlabel '"<< xlabel <<"' offset 0.0,0.0 font 'Helvetica,16'"<<endl;
        gp << "set ylabel '"<< ylabel <<"' offset 1.2,0 font 'Helvetica,16'"<<endl;
        gp <<"plot ";
        cout<<"draw:"<<name<<" : "<<title<<" size:"<<_dat.size()<<endl;
        for(unsigned int i=0;i<_dat.size();i++){
            std::vector< double > t(_dat[i].y.size());
            cout<<"   "<<_dat[i].component<<endl;
            for(unsigned int j=0;j<t.size();j++){
                t[j]=_dat[i].ti+_dat[i].dt*j;
            }
            //std::cout<<t.size()<<" max ("<<t_max<<", "<<max<<") -> "<<1.0/t_max<<std::endl;
            gp <<gp.file1d( std::make_pair(t,_dat[i].y) )<< " with lines ls "<<i+1 <<" title '"<<_dat[i].name_raw <<" : "<<_dat[i].component <<"',";
        }
        cout<<endl;
        gp <<std::endl;
    }
    //*gp <<"pause mouse close"<<endl;//https://stackoverflow.com/questions/30274786/gnuplot-wxt-not-working-anymore
}



