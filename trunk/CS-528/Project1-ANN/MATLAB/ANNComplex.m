classdef ANN
    % ANN - Implemenation of a Feedforward Neural Network
    % ANN Methods:
    %   ANN     - Creates an ANN
    %   evalANN - runs an input through the ANN
    %   trainANN - trains the ANN
    %
    % ANN Properties:
    %   Network Architecture:
    %   numLayers - number of layers (hidden)
    %   numInputs - number of inputs
    %   numOutputs - number of outputs
    %   layers - array of number of nodes at each hidden layer
    %   layerFcnNames - names of the fucntions of each hidden layer nodes
    %
    %   Implementation:
    %   inputWeights - weights of the input layer (last row is biases)
    %   layerWeights - weights for the hidden layer
    
    properties
        numLayers;              % Number of layers (including input)
        numInputs;              % Number of Inputs
        numOutputs;             % Number of Outputs
        
        weights;                % cell array of weights
        functions;              % cell array of functions
        derivatives;            % cell array of function derivatives
        layers = [];            % Nodes per layer
        trainEpochs;            % Training Epochs
        
        inputs;             % cell array of inputs
        outputs;            % cell array of outputs
    end
    
    methods
        function ann = ANN(P,T,varargin)
            % ann = ANN(P,T)
            % ann = ANN(P,T,layers,hidenFuncType)
            % Creates an feed forward Artifical Neural Network
            % Inputs:
            %   P - Matrix of patterns to train to (NumInputs x NumSamples)
            %   T - Matrix of targets (NumOutputs x NumSamples)
            %   layers - vector of number of nodes in hidden layers, i.e. a
            %       layers = [4,5] would have have 4 nodes in the first
            %       hidden layer, and 5 in the second
            %   hiddenFuncType - cell array of strings of the names of the
            %       functions to be used in the hidden layer.
            %       hiddenFuncType = {'tansig','purelin'} would have the
            %       first hidden layer be all tansig, and the second
            %       purelin
            ann.inputs = P;
            ann.outputs = T;
            [ann.numInputs TSinputs] = size(ann.inputs);
            [ann.numOutputs TSoutputs] = size(ann.outputs);
            if TSinputs ~= TSoutputs
                error('ANN:ANN','Number of pattern samples %d must match number of target samples %d\n',...
                    TSinputs,TSoutputs);
            end
            ann.layers = ann.numInputs;
            names = {'logsig'};        % Input node type
            if ~isempty(varargin)&& numel(varargin) == 2
                ann.layers = [ann.numInputs varargin{1}];
                names = {names{1},varargin{2}{:}};
            elseif ~isempty(varargin)
                warning('ANN:UnexpectedInput','Too many options provided to ANN\n');
            end
            ann.numLayers = numel(ann.layers);
            ann.layers =[ann.layers ann.numOutputs];
            
            % Setting up weights and biases
            ann.weights = cell(ann.numLayers,1);
            for i=1:ann.numLayers
                % current layer to the next layer
                ann.weights{i} = rand(ann.layers(i)+1,ann.layers(i+1));
            end
            % Setting up the layer functions
            [ann.functions ann.derivatives] = ANN.assignFunctions(names);
            ann.trainEpochs = 0;
        end
        
        function [ann,msePerf] = trainANN(ann,P,T,V)
            % ann = traingANN(ann,vararin)
            % ann = trainANN(ann,P,T,V) - User provided Pattern,
            %       Target, Validation data sets along with a performance
            %       function
            % Trains the network using back-propogation. If a validation
            % set and a performance function are not supplied than the
            % validation set is randomly chosen to be 20% of the P and T
            % data sets.
            learnRate = 1;
            mseGoal = 1E-3;
            PVal = V{1};
            TVal = V{2};
            [~,nPVal] = size(PVal);
            [~,nTVal] = size(TVal);
            if nPVal ~= nTVal
                error('ANN:PerfFun','Number of elements in Pattern and Target set must match for validation');
            end
            E = cell(nTVal,1);
            itter = 0;
            mseEpoch = Inf;
            maxItter = 10;
            msePerf = zeros(maxItter,1);
            W = cell(ann.numLayers,1);              % Weights (w/out biases)
            o = cell(ann.numLayers+1,1);            % layer outputs
            D = cell(ann.numLayers,1);              % Derivatives
            delta = cell(ann.numLayers,1);
            while mseEpoch > mseGoal && itter < maxItter
                itter = itter + 1;

                % Looping over the training examples in a random order
                [~,numSamples] = size(P);
                order = randperm(numSamples);
                for i = order
                    % Extracting the weights
                    for layer=1:ann.numLayers
                        [n,~] = size(ann.weights{layer});
                        W{layer} = ann.weights{layer}(1:(n-1),:);
                    end
                    
                    % Forward Pass
                    o{1} = P(:,i)';
                    TPred = [P(:,i); 1]';      % Adding a one on for the biases
                    for layer=1:ann.numLayers
                        f = ann.functions{layer};
                        TPred = [f(TPred*ann.weights{layer}),1];
                        N = o{layer}*W{layer};
                        o{layer+1} = f(N);   % A
                        % A  = logsig(N)
                        % dA/dN = dlogsig(N,A)
                        D{layer} = diag(ann.derivatives{layer} (N, o{layer+1}));
                    end
                    TPred(end) = [];        % Stripping the weight
                    TPred = TPred';
                    
                    % Error Computation
                    err = (TPred-T(:,i));
                    delta{end} = D{end}*err;
                    
                    for layer=(ann.numLayers-1):-1:1
                        delta{layer} = D{layer}*W{layer+1}*delta{layer+1};
                    end
                    
                    % Computing weight updates
                    for layer=1:ann.numLayers
                        dW = -learnRate*delta{layer}*[o{layer},1];
                        ann.weights{layer} = ann.weights{layer} + dW';
                    end
                end
                ann.trainEpochs = ann.trainEpochs + 1;
                
                % Computing the error
                for i=1:nTVal
                    X = [PVal(:,i); 1]';             % Adding a one on for the biases
                    for layer=1:ann.numLayers
                        X = [ann.functions{layer}(X*ann.weights{layer}),1];
                    end
                    X(end) = [];        % Stripping the weight
                    X = X';             % Returing a column vector
                    E{i} = TVal(:,i) - X;
                end
                mseEpoch = mse(E);
                msePerf(itter) = mseEpoch;
                fprintf(1,'After itteration %d the mse is %5.3e\n',itter,msePerf(itter));
            end
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
            [n,p] = size(P);
            if n ~= ann.numInputs && p ~= 1
                error('ANN:evalANN',...
                    'Evaluation input P must match the number of inputs %d\n',ann.numInputs);
            end
            % Evaluating the network
            T = [P; 1]';             % Adding a one on for the biases
            for i=1:ann.numLayers
                T = [ann.functions{i}(T*ann.weights{i}),1];
            end
            T(end) = [];        % Stripping the weight
            T = T';             % Returing a column vector
        end
    end
    
    methods (Access = private)
        function [ann] = TrainEpoch(ann,P,T,learnRate)
            % ann = TrainEpoch(P,T,learnRate)
            % Completes one training epoch of the Artifical Neutral Network
            % Training is a backpropogation, based on the presentation of
            % R. Rojas: Neural Networks, Springer-Verlag, Berlin, 1996
            % Inputs:
            %   P - Pattern / input data
            %   T - Target / output data
            %   learnRate - learning rate
            % Outputs:
            %   ann - ANN with updated weights
            W = cell(ann.numLayers,1);              % Weights (w/out biases)
            o = cell(ann.numLayers+1,1);            % layer outputs
            D = cell(ann.numLayers,1);              % Derivatives
            delta = cell(ann.numLayers,1);
            % Looping over the training examples in a random order
            [~,numSamples] = size(P);
            order = randperm(numSamples);
            for i = order
                % Extracting the weights
                for layer=1:ann.numLayers
                    [n,~] = size(ann.weights{layer});
                    W{layer} = ann.weights{layer}(1:(n-1),:);
                end
                
                % Forward Pass
                o{1} = P(:,i)';
                TPred = [P(:,i); 1]';      % Adding a one on for the biases
                for layer=1:ann.numLayers
                    f = ann.functions{layer};
                    TPred = [f(TPred*ann.weights{layer}),1];
                    N = o{layer}*W{layer};
                    o{layer+1} = f(N);   % A
                    % A  = logsig(N)
                    % dA/dN = dlogsig(N,A)
                    D{layer} = diag(ann.derivatives{layer} (N, o{layer+1}));
                end
                TPred(end) = [];        % Stripping the weight
                TPred = TPred';
                
                % Error Computation
                error = (TPred-T(:,i));
                delta{end} = D{end}*error;
                
                for layer=(ann.numLayers-1):-1:1
                    delta{layer} = D{layer}*W{layer+1}*delta{layer+1};
                end
                
                % Computing weight updates
                for layer=1:ann.numLayers
                    dW = -learnRate*delta{layer}*[o{layer},1];
                    s = sum(sum(isnan(dW)));
                    if s > 0
                        error('ANN:TrainEpoch','Weight update dW contains %d nans\n',s);
                    end
                    ann.weights{layer} = ann.weights{layer} + dW';
                end
            end
            ann.trainEpochs = ann.trainEpochs + 1;
        end % End of TrainEpoch
    end
    
    methods (Access = private, Static = true)
        
        function [fun derv] = assignFunctions(names)
            % Assins the function names and function derivates
            fun = cell(numel(names),1);
            derv = cell(numel(names),1);
            for i = 1:numel(names)
                % Assining the function type
                f = @(x) purelin(x);
                d = @(x,y) dpurelin(x,y);
                switch names{i}
                    case 'logsig'
                        f = @(x) logsig(x);
                        d = @(x,y) dlogsig(x,y);
                    case 'tansig'
                        f = @(x) tansig(x);
                        d = @(x,y) dtansig(x,y);
                    case 'hardlim'
                        f = @(x) hardlim(x);
                        d = @(x,y) dhardlim(x,y);
                    case 'purelin'
                        f = @(x) purelin(x);
                        d = @(x,y) dpurelin(x,y);
                    case 'sigmoid'
                        f = @(x) 1./(1+exp(-x));
                        d = f;
                        error('ANN:assignFunctions','sigmoid derivative not entered\n');
                    otherwise
                        warning('ANN:assignFunctions','Function %s not reconized',names{i});
                end
                fun{i} = f;
                derv{i} = d;
            end
        end
        
    end
    
end