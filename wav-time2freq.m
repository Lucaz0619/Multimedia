%open the file
fid1=fopen("C:\Users\USER\Desktop\123456\magnitude.txt","r");
fid2=fopen("C:\Users\USER\Desktop\123456\phase.txt","r");
%read the file
data1=fread(fid1,28000,'float');
data2=fread(fid2,28000,'float');
fclose(fid1);
fclose(fid2);
%create the x,y1,y2 axis
x=linspace(0,28000,28000);
y1=data1;
y2=data2;
%plot the magnitude
plot(x,y1);
xlabel('Frequency(Hz)') ;
ylabel('Magnitude(dB)') ; 
%create the png
saveas(gcf,'410586039_magnitude','png');
%plot the phase
plot(x,y2) ;
xlabel('Frequency(Hz)') ; ylabel('Phase(Degree)') ; title('Phase');
%create the png
saveas(gcf,'410586039_phase','png');