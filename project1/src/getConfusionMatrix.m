function confusionMatrix = getConfusionMatrix(prediction,real,tolerance,filename)
    
    tolPred = abs(prediction-real)<=tolerance;
    
    for i=1:length(prediction)
        prediction(i) = round(prediction(i));
        if tolPred(i)~=0
            v = prediction(i)-real(i);
            v = round(v);
            prediction(i)=real(i)+v;
        end
    end

    confusionMatrix = confusionmat(prediction,real);
    
    f = image(confusionMatrix);
    colorbar;
    saveas(f, strcat('../generated_images/',filename),'png');
    
end