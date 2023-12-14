#include "data2d.h"




Data2D::Data2D():name(""),component(""),ti(0.0),dt(1.0){}

//Data2D::Data2D(const Data2D &x) : y(x.y), name(x.name), component(x.component), ti(x.ti), dt(x.dt) {}



DataFou::DataFou()
{
    name="";
    component="";
    fi=0.0;
    df=1.0;
}
//DataFou::DataFou(const DataFou &x) : Y(x.Y), name(x.name), component(x.component), fi(x.fi), df(x.df) {}


DataFou fou(const Data2D &A){
    DataFou B;
    B.name="Fou( "+A.name+" )";
    B.name_raw=A.name_raw;
    B.component=A.component;
    B.df=1.0/(A.dt*A.y.size());
    Eigen::FFT< double > fft;
    fft.fwd(B.Y, A.y);
    return B;
}

Data2D Derive( const Data2D &A){
    Data2D B(A);
    B.name="Derive( "+A.name+" )";
    B.name_raw=A.name_raw;

    Eigen::FFT< double > fft;
    std::vector< std::complex<double> > c;
    fft.fwd(c, A.y);
    for(unsigned int j=0;j<c.size();j++){
        std::complex<double> H(0.0,j);//derivada
        c[j]=H*c[j];
    }
    fft.inv( B.y,c);
    B.max=max_element_abs( B.y );
    return B;
}

Data2D Integrate( const Data2D &A){
    Data2D B(A);
    B.name="Integrate( "+A.name+" )";
    B.name_raw=A.name_raw;
    Eigen::FFT< double > fft;
    std::vector< std::complex<double> > c;
    fft.fwd(c, A.y);
    c[0]=std::complex<double>(0.0,0.0);

    for(unsigned int j=1;j<c.size();j++){
        //std::complex<double> H(0.0,-1.0/j);//integra
        double f=(double)j/(A.dt*A.y.size());
        std::complex<double> H(0.0,-1.0/f);//integra
        c[j]=H*c[j]/(2.0*M_PI);
    }
    fft.inv( B.y,c);
    B.max=max_element_abs( B.y );
    return B;
}

Data2D IntegrateTrapecio( const Data2D &A){
    Data2D B(A);
    B.name="IntegrateTrapecio( "+A.name+" )";
    B.name_raw=A.name_raw;
    double s=0.0;
    B.y[0]=0.0;
    for(unsigned int j=1;j<A.y.size();j++){
        s+=A.dt*(A.y[j-1]+A.y[j])/2.0;
        B.y[j]=s;
    }
    B.max=max_element_abs( B.y );
    return B;
}

Data2D Integrate2( const Data2D &A){
  Data2D B(A);
  B.name="Integrate2( "+A.name+" )";
  B.name_raw=A.name_raw;
  Eigen::FFT< double > fft;
  std::vector< std::complex<double> > c;
  fft.fwd(c, A.y);
  c[0]=std::complex<double>(0.0,0.0);
  for(unsigned int j=1;j<c.size();j++){
      //std::complex<double> H(0.0,-1.0/j);//integra
      double f=(double)j/(A.dt*A.y.size());
      double H=-1.0/(f*f);
      c[j]=H*c[j]/(2.0*M_PI)/(2.0*M_PI);
  }
  fft.inv( B.y,c);
  B.max=max_element_abs( B.y );
  return B;
}

Data2D Lowpass( const Data2D &A, double Fmax,int NPolos){
  Data2D B(A);
  B.name="Lowpass( "+A.name+" )";
  B.name_raw=A.name_raw;
  Eigen::FFT< double > fft;
  std::vector< std::complex<double> > c;
  fft.fwd(c, A.y);
  c[0]=std::complex<double>(0.0,0.0);
  for(unsigned int j=1;j<c.size();j++){
      //std::complex<double> H(0.0,-1.0/j);//integra
      double f=(double)j/(A.dt*A.y.size());
      double H=1.0/sqrt(1.0+pow(f/Fmax,2.0*NPolos));
      c[j]=H*c[j];
  }
  fft.inv( B.y,c);
  B.max=max_element_abs( B.y );
  return B;
}

Data2D Highpass( const Data2D &A, double Fmin,int NPolos){
  Data2D B(A);
  B.name="Highpass( "+A.name+" )";
  B.name_raw=A.name_raw;
  Eigen::FFT< double > fft;
  std::vector< std::complex<double> > c;
  fft.fwd(c, A.y);
  c[0]=std::complex<double>(0.0,0.0);
  for(unsigned int j=1;j<c.size();j++){
      double f=(double)j/(A.dt*A.y.size());
      double H=1.0/sqrt(1.0+pow(Fmin/f,2.0*NPolos));
      c[j]=H*c[j];
  }
  fft.inv( B.y,c);
  B.max=max_element_abs( B.y );
  return B;
}



//void Bandpass( std::vector<std::complex<double> > &X, double T, double Fmin, double Fmax, int NPolos=4){
Data2D Bandpass( const Data2D &A, double Fmin, double Fmax,int NPolos){
  //int N=X.size()/2;
  //X[0]=std::complex<double>(0.0,0.0);
  
  //cout<<"B(A)"<<endl;
  Data2D B(A);
  B.name="Bandpass( "+A.name+" )";
  B.name_raw=A.name_raw;
  Eigen::FFT< double > fft;
  std::vector< std::complex<double> > c;
  //for(int i=0;i<A.y.size();i++)cout<<i<<" "<<A.y[i]<<endl;
  //cout<<"fft.fwd(c, A.y);1"<<endl;
  fft.fwd(c, A.y);
  //cout<<"fft.fwd(c, A.y);2"<<endl;
  //for(int i=0;i<c.size();i++)cout<<i<<" "<<c[i]<<endl;
  c[0]=std::complex<double>(0.0,0.0);
  //cout<<"for1"<<endl;
  for(unsigned int j=1;j<c.size();j++){
    //double f=(double)j/T;
      double f=(double)j/(A.dt*A.y.size());
    double H=1.0/sqrt(1.0+pow((f*f-Fmin*Fmax)/(f*(Fmax-Fmin)),2.0*NPolos));
    c[j]=H*c[j];
  }
  //cout<<"for2"<<endl;
  //cout<<"fft.inv( B.y,c);1"<<endl;
  fft.inv( B.y,c);
  //cout<<"fft.inv( B.y,c);2"<<endl;
  //cout<<"max_element_abs1"<<endl;
  B.max=max_element_abs( B.y );
  //cout<<"max_element_abs2"<<endl;
  return B;
}

Data2D abs(const DataFou &A){
    Data2D B;
    B.name="Abs( "+A.name+" )";
    B.name_raw=A.name_raw;
    B.component=A.component;
    B.dt=A.df;
    int N=A.Y.size()/2;
    B.y.resize(N);
    for(int i=0;i<N;i++){
        //B.y[i]=std::abs( A.Y[i] )/100.0;
        B.y[i]=std::abs( A.Y[i] )/(A.df*2*N);

        //1.0/(A.dt*A.y.size());
        //B.y[i]=sqrt( A.Y[i].real()*A.Y[i].real()+A.Y[i].imag()*A.Y[i].imag() )/N;
    }
    //std::cout<<"max="<<max<<std::endl;

    //std::complex<double> xxx;
    //xxx.real();
    //xxx.imag();

    B.max=max_element_abs( B.y );
    return B;
}

Data2D smoothing(const Data2D& A, int Fs){
    Data2D B(A);
    B.name="Smth( "+A.name+" )";
    B.name_raw=A.name_raw;
    B.component=A.component;
    int N=A.y.size();
    for(int j=0;j<N;j++){
        int k1=round((double)j*pow(2.0,-0.5/Fs));
        int k2=round((double)j*pow(2.0, 0.5/Fs));
        if(k1<0)k1=0;
        if(k2>N-1)k2=N-1;
        int Nk=k2-k1+1;
        double SAf2=0.0;
        //    for(int k=k1;k<=k2;k++){
        //      SAf2+=(A.y[k]*A.y[k]);
        //    }
        for(int i=0;i<Nk;i++){
            SAf2+=(A.y[k1+i]*A.y[k1+i]);
        }
        double As=sqrt(SAf2/Nk);
        //    if(j<20)cout<<j<<" As="<<As<<" k1="<<k1<<" k2="<<k2<<" Nk="<<Nk<<" SAf2="<<SAf2<<endl;
        B.y[j]=As;
    }
    B.max=max_element_abs( B.y );
    return B;
}

Data2D taper(const Data2D &A, double R){
    Data2D B(A);
    B.name="Taper( "+A.name+" )";
    B.name_raw=A.name_raw;
    B.component=A.component;
    double a=R/100.0;
    int N=A.y.size();
    for(int j=0;j<N;j++){
        double t=(double)j/(N-1);
        double s=0.0;
        if(t>=0.0 && t<a){
            s=0.5*(1.0-cos(M_PI*t/a));
        }
        else{
            if(a<=t && t<=(1.0-a)){
                s=1.0;
            }
            else{
                if(t>(1.0-a) && t<=1.0){
                    s=0.5*(1.0-cos(M_PI*(1.0-t)/a));
                }
            }
        }
        B.y[j]=s*A.y[j];
    }
    B.max=max_element_abs( B.y );
    return B;
}

/*
void Abs(std::vector<double> &z, const std::vector<std::complex<double> > &Z){
  int N=Z.size()/2;
  z.resize(N);
  double max=-1.0;
  for(int j=0;j<N;j++){
    z[j]=std::abs( Z[j] )/N;
    max=(z[j]>max)?z[j]:max;
  }
  std::cout<<"maximo="<<max<<std::endl;
}
*/


Data2D ResponseSpectrum(Data2D& A,int NT,double Tmin,double Tmax,double z){
    Data2D B;
    B.name="ResSpec( "+A.name+" )";
    B.name_raw=A.name_raw;
    B.component=A.component;
    B.y.resize(NT);
    B.ti=Tmin;
    B.dt=(Tmax-Tmin)/(NT-1);
    double sz=sqrt(1.0-z*z);

    for(int j=0;j<NT;j++){

        double Tn=Tmin+(Tmax-Tmin)*j/(NT-1);
        double wn=2.0*M_PI/Tn;
        double ES=exp(-z*wn*A.dt)*sin((sz*wn*A.dt));
        double EC=exp(-z*wn*A.dt)*cos((sz*wn*A.dt));

        double ui,ui1;
        double duidt,dui1dt;



        double A1=z*ES/sz + EC;
        double B1=ES/(sz*wn);
        double C1=( 2.0*z/(wn*A.dt) + ( ((1.0-2.0*z*z)/(sz*wn*A.dt) - z/sz)*ES - (1.0+2.0*z/(wn*A.dt))*EC ) )/(wn*wn);
        double D1=( 1.0 - 2.0*z/(wn*A.dt) + ( (2.0*z*z-1.0)*ES/(sz*wn*A.dt) + 2.0*z*EC/(wn*A.dt) ) )/(wn*wn);
        double A2=-wn*ES/sz;
        double B2=-z*ES/sz + EC;
        double C2= -( 1.0 - ( A.dt*wn*ES/sz + z*ES/sz + EC ) )/(wn*wn*A.dt);
        double D2=  ( 1.0 - (                z*ES/sz + EC ) )/(wn*wn*A.dt);



        ui=0.0;
        duidt=0.0;
        double umax=0.0;
        for(unsigned int i=0;i<A.y.size()-1;i++){
            //ui1=A1*ui+B1*duidt-C1*A.y[i]-D1*A.y[i+1];
            //dui1dt=A2*ui+B2*duidt-C2*A.y[i]-D2*A.y[i+1];
            ui1=A1*ui+B1*duidt-C1*A.y[i]-D1*A.y[i+1];
            dui1dt=A2*ui+B2*duidt-C2*A.y[i]-D2*A.y[i+1];
            ui=ui1;
            duidt=dui1dt;
            if(umax<abs(ui1))umax=abs(ui1);
        }

        B.y[j]=umax*(2.0*M_PI/Tn)*(2.0*M_PI/Tn);
    }
    B.max=max_element_abs( B.y );
    return B;

}


double ResponseSpectrum(Data2D& A,double T,double z){
    double sz=sqrt(1.0-z*z);
    double Tn=T;
    double wn=2.0*M_PI/Tn;
    double ES=exp(-z*wn*A.dt)*sin((sz*wn*A.dt));
    double EC=exp(-z*wn*A.dt)*cos((sz*wn*A.dt));
    double ui,ui1;
    double duidt,dui1dt;

    double A1=z*ES/sz + EC;
    double B1=ES/(sz*wn);
    double C1=( 2.0*z/(wn*A.dt) + ( ((1.0-2.0*z*z)/(sz*wn*A.dt) - z/sz)*ES - (1.0+2.0*z/(wn*A.dt))*EC ) )/(wn*wn);
    double D1=( 1.0 - 2.0*z/(wn*A.dt) + ( (2.0*z*z-1.0)*ES/(sz*wn*A.dt) + 2.0*z*EC/(wn*A.dt) ) )/(wn*wn);
    double A2=-wn*ES/sz;
    double B2=-z*ES/sz + EC;
    double C2= -( 1.0 - ( A.dt*wn*ES/sz + z*ES/sz + EC ) )/(wn*wn*A.dt);
    double D2=  ( 1.0 - (                z*ES/sz + EC ) )/(wn*wn*A.dt);

    ui=0.0;
    duidt=0.0;
    double umax=0.0;
    for(unsigned int i=0;i<A.y.size()-1;i++){
        ui1=A1*ui+B1*duidt-C1*A.y[i]-D1*A.y[i+1];
        dui1dt=A2*ui+B2*duidt-C2*A.y[i]-D2*A.y[i+1];
        ui=ui1;
        duidt=dui1dt;
        if(umax<abs(ui1))umax=abs(ui1);
    }

    return umax*(2.0*M_PI/Tn)*(2.0*M_PI/Tn);
}


