CLAV = [0.02390079993642359900	      0.00664092089472755050	      0.02620252334065026000]';

C7 = [-0.06487653065255633000	      0.04469330006262592400	     -0.00151000000000000010]';

RSHO = [-0.01914358849076480700	      0.02679361453611674900	      0.17306094484963150000]';  % shoulder

RUPA = [0.00416068370452796900	     -0.18412012212213649000	      0.20572949567625259000]';  % biceps

RELB = [-0.00712458911239045520	     -0.27515427823046779000	      0.25860659789537671000]'; % lateral in modal (actually seems to be in the middle)

RFRM = [0.11366949630764399000	     -0.22360483650980606000	      0.22835117321315174000]'; %forearm

RRAD = [0.22084328918258225000	     -0.17928735625866926000	      0.22606094069439756000]'; %radius

HAND = [0.32870587528783068000	     -0.13644930603330099000	      0.22414202952313142000]'; %hand

RELM = [-0.00284889979896889300	     -0.29652688701423541000	      0.17798687962080625000]'; %elbow medial (at the side in the model)

RULN = [0.23451089519267002000	     -0.23207611833125641000	      0.27215812378889914000]'; % ulna



    
if (norm(C7-CLAV)<0.2)&&(norm(CLAV-RSHO)>0.08) % sanity check, add to this if necessary

    z_t_temp = [0;1;0];
    x_t = (RSHO-CLAV)/norm(RSHO-CLAV);
    y_t_temp = (cross_efficient(z_t_temp,x_t));
    y_t = y_t_temp/norm(y_t_temp);
    z_t = cross_efficient(x_t,y_t);


    R = [x_t, y_t, z_t];
%         if (z_t(3)<0) % if its upside down
%             R = -R;
%         end

    T(1:3,1:3) = R;
    T(1:3,4) = RSHO;   
    T(4,:) = [0,0,0,1];
    error = 0;
    iT = inv(T);
    iT(3,4) = iT(3,4)+0.040;

else                        % sonst ergibt die TrafoMatrix keinen Sinn, und wir k?nnen nichts berechnen.
    T=zeros(4,4);
    error = 1;
end    

    
    
    
    
    %% Matlab Skripte zur Berechnung von S, E, W
% needs:
% RSHO - shoulder marker
% CLAV -  clavicle (Schl?sselbein) marker an den Nacken
% RELB - elbow marker (lateral)
% RRAD - radius
% RULN - ulna
 
z_temp = [0;1;0];   % senkrecht im Modell
x = (RSHO-CLAV)'/norm(RSHO-CLAV);  % entlang des Schl?sselbeins
y_temp = (cross(z_temp,x)')'; 
y = y_temp/norm(y_temp);
z = cross(x,y);
S = [0;0;0];  % ein bisschen unter dem Schulter
 
Ehom = iT*[RELB;1];
E = Ehom(1:3,1);
 
Whom = iT*[(RRAD + RULN)/2;1];
W = Whom(1:3,1);
