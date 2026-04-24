% =========================================================
% This was made for a component measured with only ONE port, if your
% component was measured with TWO ports in series your Rs will be Rs+Z0
% =========================================================

% Your data should have 3 columns [frecuency, S11 real, S11 imaginary]

fnom=5e6; % <--------------Enter the frequency at which you designed your inductor
f = datos(:,1);
re = datos(:,2);
im = datos(:,3);


Gamma = re + 1i * im;
mag_S11 = abs(Gamma);
S11_dB = 20 * log10(mag_S11);

S11_fase_deg = rad2deg(angle(Gamma));
Z0 = 50;
Z = Z0 * (1 + Gamma) ./ (1 - Gamma);

R = real(Z);
X = imag(Z);
Xfreq=X./(2*pi*f);

[~, indxL] = min(abs(datos(:, 1) - fnom));
Lnom=Xfreq(indxL); % Obtains measured inductance
cambios_signo = find(diff(sign(X)) < 0); 

%---Takes frequency in the zero cross
if ~isempty(cambios_signo)
    idx = cambios_signo(1);
    f_srf = f(idx) - X(idx) * (f(idx+1) - f(idx)) / (X(idx+1) - X(idx));
end

%---Calculates Cp, Rs
syms C;
syms Rnom;
Cpar= double(vpasolve(1/sqrt(C*Lnom)==2*pi*f_srf));
w_srf = 2*pi*f_srf;
Z_serie = Rnom + 1i*w_srf*Lnom;
Y_paralelo = 1i*w_srf*Cpar;
Zmod = Z_serie/(1 + Z_serie* Y_paralelo);
[~, indxR] = min(abs(datos(:, 1) - f_srf));
Rmod=real(double(vpasolve((Zmod-Z0)/(Zmod+Z0)==Gamma(indxR),Rnom)));

Z_sermod=Rmod+1i*2*pi.*f*Lnom;
Y_parmod=1i*2*pi.*f*Cpar;
Zcal=Z_sermod./(1 + Z_sermod.* Y_parmod);
Xfreqcal=imag(Zcal)./(2*pi*f);
%---Resistance
subplot(3,1,1);
plot(f, R, 'b', 'LineWidth', 1); hold on;        % Measured
ylim([-2*max(R),2*max(R)]);
plot(f, real(Zcal), 'r--', 'LineWidth', 1.5);    % Modeled
title('Resistencia Serie (R) [\Omega]');
ylabel('\Omega');
legend('R medido', 'R modelo');
grid on;
%---Admitance
subplot(3,1,2);
plot(f, X, 'b', 'LineWidth', 1); hold on;        % Measured
ylim([-2*max(X),2*max(X)]);
plot(f, imag(Zcal), 'r--', 'LineWidth', 1.5);    % Modeled
title('Reactancia (X) [\Omega]');
ylabel('\Omega');
legend('X medido', 'X modelo');
grid on;
%---Inductance
subplot(3,1,3);
plot(f, Xfreq, 'b', 'LineWidth', 1); hold on;        % Measured
plot(f, Xfreqcal, 'r--', 'LineWidth', 1.5);    % Modeled
title('Inductancia (L) [H]');
ylabel('H');
legend('L medido', 'L modelo');
grid on;
disp('Resultados del Modelo [Rs,Cp,L]:');
fprintf('Rs: %.3e Ohm, Cp: %.3e F, L: %.3e H\n', Rmod, Cpar, Lnom);