clc
clear all
close all
datos = readtable('Mediciones JDSU.xlsx');
x = table2array(datos(:,1));
y = table2array(datos(:,2));
polinomioGrafica = polyfit(x,y,10);
x1 = min(x):max(x);
y1 = polyval(polinomioGrafica,x1);
figure
plot(x,y,'o')
hold on
plot(x1,y1)
hold off
xlim([min(x1)-5 max(x1)+5])
ylim([0 max(y1)+1])
xlabel('Divisiones ADC 12 bits','FontSize',13)
ylabel('Atenuación [dB]','FontSize',13)
title('Respuesta EVOA','FontSize',14)
legend({'Mediciones','Aproximación Polinómica'},'Location','northwest','FontSize',12)
legend('boxoff')
grid on
xLookup = 205:1:3890;
yLookup = polyval(polinomioGrafica,xLookup);
yLookup = 10*int32(yLookup*10);
figure;
plot(xLookup, yLookup)
xlabel('Divisiones ADC 12 bits','FontSize',13)
ylabel('Atenuación [dB]','FontSize',13)
title('Aproximación Lineal Grado 10','FontSize',14)
legend('Aproximación Polinómica','Location','northwest','FontSize',12)
legend('boxoff')
grid on
tLookup = table(yLookup);
writetable(tLookup, 'LookupTable.txt','Delimiter',',','WriteRowNames',false);








