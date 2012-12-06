classdef ANN
    % ANN - Implemenation of a Feedforward Neural Network
    % ANN Methods:
    %   ANN     - Creates an ANN
    %   evalANN - runs an input through the ANN
    %   trainANN - trains the ANN
    %
    % ANN Properties:
    %   Network Architecture:
    %   numNodesHidden - number of hidden nodes
    %   numInputs - number of inputs
    %   numOutputs - number of outputs
    %
    %   Implementation:
    %   W1Bar - weights of the input layer (last row is baises)
    %   W2Bar - weights for the hidden layer
    %
    % Author:  Matthew J. Urffer (matthew.urffer@gmail.com)
    % $Date: 9/27/2012
    
    properties
        W1;                     % Weight matrix of first layer
        W2;                     % Weight matrix of second layer
        n;                      % Number of input nodes
        k;                      % Number of hidden nodes
        m;                      % Number of output nodes
        
        hiddenFcn;              % Hidden Function Name
        performance;            % Use hard coded function names
        
        outputFcn;
        trainEpochs;            % Training Epochs
        learnRate;
        
        inputs;             % cell array of inputs
        outputs;            % cell array of outputs
    end
    
    methods
        function ann = ANN(P,T,k)
            % ann = ANN(P,T)
            % ann = ANN(P,T,numHiddenNodes)
            % Creates an feed forward Artifical Neural Network
            % Inputs:
            %   P - Matrix of patterns to train to (NumInputs x NumSamples)
            %   T - Matrix of targets (NumOutputs x NumSamples)
            %   k - number of nodes in hidden layers
            
            ann.inputs = P;
            ann.outputs = T;
            ann.k = k;
            [ann.n TSinputs] = size(ann.inputs);
            [ann.m TSoutputs] = size(ann.outputs);
            if TSinputs ~= TSoutputs
                error('ANN:ANN','Number of pattern samples %d must match number of target samples %d\n',...
                    TSinputs,TSoutputs);
            end
            
            % Setting up layer functions
            ann.hiddenFcn = 'logsig';
            ann.outputFcn = 'softmax';
            ann.performance = true;
            
            % Setting up weights and biases
            randScale = 0.2;
            sizeW1 = [ann.n+1,k];
            sizeW2 = [k+1,ann.m];
            ann.W1 = randScale*rand(sizeW1)-randScale*0.5*ones(sizeW1);
            ann.W2 = randScale*rand(sizeW2)-randScale*0.5*ones(sizeW2);
            
            ann.learnRate = 0.2;
            ann.trainEpochs = 0;
        end
        
        function [ann,errorPerformance] = trainANN(ann,P,T,V,mseGoal)
            % ann = trainANN(ann,P,T,V) - User provided Pattern,
            %       Target, Validation data sets along with a performance
            %       function
            % Trains the network using back-propogation. If a validation
            % set and a performance function are not supplied than the
            % validation set is randomly chosen to be 20% of the P and T
            % data sets.
            % Based on the presentation of
            % R. Rojas: Neural Networks, Springer-Verlag, Berlin, 1996
            
            PVal = V{1};
            TVal = V{2};
            [~,nPVal] = size(PVal);
            [~,nTVal] = size(TVal);
            if nPVal ~= nTVal
                error('ANN:PerfFun','Number of elements in Pattern and Target set must match for validation');
            end
            [~,numSamples] = size(P);
            
            itter = 0;
            mseEpoch = Inf;
            maxItter = 500;
            msePerf = cell(maxItter,1);                % mse performance per epoch (on validation set)
            ssePerf = cell(maxItter,1);          % sse performance per epoch (on validation set)
            ssePerfItter = zeros(maxItter*numSamples,1);% sse performance per itteration (on training set)
            fprintf(1,'Starting Training\n');
            while mseEpoch > mseGoal && itter < maxItter
                
                % Looping over the training examples in a random order
                order = randperm(numSamples);
                
                for i = order
                    
                    % Forward Pass
                    X = P(:,i)';
                    if ann.performance
                        N1 = [X,1]*ann.W1;
                        o1 = logsig(N1);
                        N2 = [o1,1]*ann.W2;
                        o2 = softmax(N2');
                    else
                        N1 = [X,1]*ann.W1;
                        o1 = feval(ann.hiddenFcn,N1);
                        N2 = [o1,1]*ann.W2;
                        o2 = feval(ann.outputFcn,N2');
                    end
                    % Explicity caluclating the derivate as a*(1-a).  This
                    % is done because the dlogsig,dsoftmax seem to be
                    % giving wierd output.
                    D1 = diag(o1.*(ones(size(o1))-o1));
                    D2 = diag(o2.*(ones(size(o2))-o2));
                    
                    
                    % Error Computation - Note that the basis is excluded
                    % in delta1
                    err = (o2-T(:,i));
                    delta2 = D2*err;
                    delta1 = D1*ann.W2(1:(end-1),:)*delta2;
                    
                    ann.W2 = ann.W2 + (-ann.learnRate*delta2*[o1,1])';
                    ann.W1 = ann.W1 + (-ann.learnRate*delta1*[X,1])';
                    ssePerfItter(i*itter+i) = norm(err);
                    
                end
                itter = itter + 1;
                ann.learnRate = ann.learnRate*exp(-itter/(maxItter^2));
                ann.trainEpochs = ann.trainEpochs + 1;
                
                % Computing the error
                [msePerf{itter} ssePerf{itter}] = ann.validateANN(PVal,TVal);
                fprintf(1,'After itteration %d the mse is %5.3e and the sse is %5.1f\n',...
                    itter,msePerf{itter},ssePerf{itter}(end));
                mseEpoch = msePerf{itter};
            end
            errorPerformance = struct('mseValSetPerEpoch',{msePerf},...
                'sseValSetPerEpoch',{ssePerf},'sseTrainSetPerItter',{ssePerfItter});

        end
        
        function [mseEpoch, sseEpoch] = validateANN(ann,PVal,TVal)
            % [mseEpoch,sseEpoch] = validateANN(ann,PVal,TVal)
            %
            % Computes the validation (summerized in the Sum Square Error
            % and Mean Squared Error)
            % Inputs:
            %   PVal - Pattern validation data set
            %   TVal - Target valdiation data set
            % Outputs:
            %   mseEpoch - mean squared error accoring to mse
            %   sseEpoch - sum squared error according to sse
            
            [h,nTVal] = size(TVal);
            sseEpoch = zeros(1,h+1);
            E = cell(nTVal,1);
            for i=1:nTVal
                X = PVal(:,i)';
                if ann.performance
                    X = logsig([X,1]*ann.W1);
                    X = softmax(([X,1]*ann.W2)');
                else
                    X = feval(ann.hiddenFcn,[X,1]*ann.W1);
                    X = feval(ann.outputFcn,([X,1]*ann.W2)');
                end
                E{i} = TVal(:,i) - X;
                sseEpoch(1:10) = E{i}.^2;
            end
            mseEpoch = mse(E);
            sseEpoch(end) = sse(E);
        end
        
        function T = evalANN(ann,P)
            % T = evalANN(ANN,P)
            % Evaluates the ANN on the input patter P, returing the target
            % class T
            % Inputs:
            %   ANN - Network
            %   P - Pattern to run through the network
            % Output:
            %   T - Target vector T
            if ann.performance
                T = logsig([P',1]*ann.W1);
                T = softmax(([T,1]*ann.W2)');
            else
                T = feval(ann.hiddenFcn,[P',1]*ann.W1);
                T = feval(ann.outputFcn,([T,1]*ann.W2)');
            end
            
        end
        
        
        function [m,order,accuracy] = ConfusionMatrix(ann,P,T,classNames)
            % [m,accuracy] = ConfusionMatrix(ann,P,T)
            % Calculates the confusion matrix of an ANN, given inputs
            % patterns P and corresponding actual targets T
            
            [~,numSamples] = size(T);
            
            predictedClass = zeros(numSamples,1);
            actualClass = zeros(numSamples,1);
           
            for i=1:numSamples
                [~,I] = max(ann.evalANN(P(:,i)));
                predictedClass(i) = classNames{I};
                [~,I] = max(T(:,i));
                actualClass(i) = classNames{I};
            end
            [m,order] = confusionmat(actualClass,predictedClass);
            accuracy = trace(m)/sum(sum(m));
        end
    end
    
    methods (Static = true)
        
        function plotPerformance(perf,classNames)
            % Plots the performance of a given performance strucutre
            msePerf = perf.mseValSetPerEpoch;
            ssePerf = perf.sseValSetPerEpoch;
            sseTrainSet = perf.sseTrainSetPerItter;
            epochs =1:numel(msePerf);
            t = cell2mat(ssePerf);
            h = numel(sseTrainSet)/numel(msePerf);
            itters = (1:numel(sseTrainSet))/h;
            
            % Plotting the MSE Permance
            figure(2)
            semilogy(epochs,cell2mat(msePerf));
            xlabel('Epoch')
            ylabel('Mean Squared Error');
            
            
            % Plotting SSE per output
            figure(3);
            semilogy(epochs,t(:,1:end-1)');
            legend(classNames);
            xlabel('Epoch');
            ylabel('Sum Squared Error');
            
            % Plotting the Total SSE Performance
            figure(4);
            semilogy(epochs,t(:,end));
            hold all;
            semilogy(itters,sseTrainSet);
            hold off;
            xlabel('Epoch');
            ylabel('Sum Squared Error');
            legend('Training Set','Validation Set');   
        end
    end
end