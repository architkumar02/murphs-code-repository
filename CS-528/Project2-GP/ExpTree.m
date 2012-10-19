classdef ExpTree
    
    properties(GetAccess = public, SetAccess = private)
        binaryTree;                     % Binary Tree as Cell Array
        maxDepth;                       % Maximum Depth of the tree
        maxNodes;                       % Maximum number of nodes
        functionSet = containers.Map(); % Function Set, and probability
        terminalSet = containers.Map(); % Teriminal Set, and probability
    end
    %% Start of Public Methods
    methods
        
        function obj = ExpTree(varargin)
            % ExpTree - Creates an expression tree
            %   Inputs:
            %       ExpTree() - default expression tree of depth 5
            %           The function set is equal probabilities of sin,
            %           cos, and exp with +,-,/,*,^.
            %           The terminal set is x,1,0 and pi
            %       ExpTree(depth,functionSetMap,terminalSetMap)
            %           depth - depth of the tree
            %           functionSetMap - Map between function and
            %               probability of occuring
            %           terminalSetMap - Map between terminal and
            %               probability of occuring
            if ~nargin
                % Default Function Set
                fSet = {'sin','cos','exp','power','plus','minus','mldivide','times'};
                fProbability = ones(numel(fSet),1)/numel(fSet);
                fSetMap = containers.Map(fSet,fProbability);
                
                % Default Terminal Set
                tSet = {'x','1','pi'};
                tProbability = ones(numel(tSet),1)/numel(tSet);
                tSetMap = containers.Map(tSet,tProbability);
                
                % Calling Private constructor
                obj = obj.RandomExpTree(6,fSetMap,tSetMap);
            else
                % Better hope that the user provided the correct arguments
                obj = obj.RandomExpTree(varargin{1},varargin{2},varargin{3});
            end
            
        end
        
        function value = eval(exprTree,x,varargin)
            % value = eval(exprTree,x)
            % value = eval(exprTreee,x,zeroValue)
            %    Evaluates an expression tree given the value of x.  If
            %    zeroValue is provided, that value is used to replace any
            %    zeros that appear in the expression.
            if isempty(varargin)
                zeroValue = pi;
            else
                zeroValue = varargin{1};
            end
            t = exprTree.binaryTree;
            internalNodes = 2^(exprTree.maxDepth)-1;
            value = Inf;
            for node=internalNodes:-1:1
                pC = t{node};
                if ~isempty(pC)
                    lC = t{2*node};
                    rC = t{2*node+1};
                    if exprTree.terminalSet.isKey(pC{1})
                        s = sprintf('%s',pC{1});
%                         fprintf(1,'Evaluated node %d with %s =',node,s);
                        value = eval(s);
%                         fprintf(1,'%5.3f\n',value);
                        
                    elseif ~isempty(lC) && ~isempty(rC)
                        s = sprintf('%s(%s,%s)',pC{1},lC{1},rC{1});
%                         fprintf(1,'Evaluated node %d with %s =',node,s);
                        value = eval(s);
%                         fprintf(1,'%5.3f\n',value);
                        %                         if value == 0
                        %                             value = zeroValue;
                        %                         end
                    elseif ~isempty(rC)
                        s = sprintf('%s(%s)',pC{1},rC{1});
%                         fprintf(1,'Evaluated node %d with %s =',node,s);
                        value = eval(s);
%                         fprintf(1,'%5.3f\n',value);
                        %                         if value == 0
                        %                             value = zeroValue;
                        %                         end
                    end
                    t{node}= {sprintf('%10.8e',value)};
                    
                end
            end
        end
        
        function subTree = subTree(exprTree,node)
            % subTree = subTree(exprTree,node)
            %    Returns an expression the subtree of an expression tree,
            %    using node as the new root
            warning('subTree:ExpTree','Method subTree is not implemented\n');
        end
        
        function [tf] = equals(tree1,tree2)
            % tf = equals(tree1,tree2)
            %    Test for tree inequality by strict comparison of the the
            %    cell arrays
            tf = isequal(tree1.binaryTree,tree2.binaryTree);
        end
        
        function [numNodes,nodes] = getNumNodesAtDepth(exprTree,depth)
            % [numNodes,nodeRange] = getNumNodesAtDepth(exprTree,depth)
            %   Returns:
            %       numNodes - the number of non-empty nodes at that depth
            %       nodeRange - all possible nodes at that depth
            nodeRange = 2^(depth):(2^(depth+1)-1);
            nodes = cell(numel(nodeRange),1);
            numNodes = 0;
            for node = nodeRange
                if ~isempty(exprTree.binaryTree{node})
                    numNodes = numNodes +1;
                    nodes{node} = node;
                end
            end
            % Removing the empty spaces
            nodes = nodes(~cellfun(@isempty,nodes));
        end
        
        function [t1,t2] = swap(t1,t2,depth)
            % [t1,t2] = swap(t1,t2,depth)
            %   Swaps the two trees t1 and t2 from a node choosen at random
            %   from a given depth
            [numNodes1,nodes1] = getNumNodesAtDepth(t1,depth);
            [numNodes2,nodes2] = getNumNodesAtDepth(t2,depth);
            if (isempty(numNodes1) || numNodes1 == 0 || ...
                    isempty(numNodes2) || numNodes2 == 0)
                return;
            end
            node1 = nodes1{randi(numNodes1)};
            node2 = nodes2{randi(numNodes2)};
            
            swapNodes1 = t1.getChildren(node1,t1.maxNodes,[]);
            swapNodes2 = t1.getChildren(node2,t2.maxNodes,[]);
            
            for n = 1:numel(swapNodes1)
                temp = t1.binaryTree{swapNodes1(n)};
                t1.binaryTree{swapNodes1(n)} = t2.binaryTree{swapNodes2(n)};
                t2.binaryTree{swapNodes2(n)} = temp;
            end
            
        end
        
        
        function exprTree = prune(exprTree,depth)
            % exprTree = prune(exprTree,depth)
            %   Prunes (removes) all chidren from a node randomly selected
            %   at the supplied depth. The node is then set to a randomly
            %   drawn leaf.
            [numNodes,nodes] = getNumNodesAtDepth(exprTree,depth);
            if numNodes == 0
                return;
            end
            node = nodes{randi(numNodes)};
            deadNodes = ExpTree.getChildren(node,exprTree.maxNodes,[]);
            for n = deadNodes
                exprTree.binaryTree{n} = [];
            end
            
            % Setting the node to be a leaf
            exprTree.binaryTree{node} = exprTree.chooseTerminal();
        end
        
        function exprTree = mutate(exprTree,mutationRate)
            % exprTree = mutate(exprTree,nodeFraction)
            %    Inputs:
            %        muationRate - probability that a leaf will be mutated
            
            firstLeaf = 2^exprTree.maxDepth;
            for node = firstLeaf:exprTree.maxNodes
                if rand() < mutationRate
                    exprTree.binaryTree{node} = exprTree.chooseTerminal();
                end
            end
        end
        
        function WriteExprTree(exprTree,fileName)
            % Writes the ExprTree to fileName.  The format is .dot, which
            % can be read by Graphviz.
            
            f = fopen(fileName,'w');
            
            % Write Header
            fprintf(f,'strict digraph G {\n');
            
            % Write Node Labels
            for node=1:exprTree.maxNodes
                nodeContent = exprTree.binaryTree{node};
                if ~isempty(nodeContent)
                    fprintf(f,'%d [label="%s"];\n',node,nodeContent{1});
                end
            end
            
            % Write tree stucture, skipping the leafs
            internalNodes = 2^(exprTree.maxDepth)-1;
            for node=1:internalNodes
                nodeContent = exprTree.binaryTree{node};
                nodeContentLeft = exprTree.getLeft(node);
                nodeContentRight = exprTree.getRight(node);
                if ~isempty(nodeContent)
                    if ~isempty(nodeContentLeft)
                        fprintf(f,'%d -> %d;\n',node,2*node);
                    end
                    if ~isempty(nodeContentRight)
                        fprintf(f,'%d -> %d;\n',node,2*node+1);
                    end
                end
            end
            % Writer Footer
            fprintf(f,'}\n');
            fclose(f);
        end
    end
    %% Start of Static Methods
    methods (Static = true)
        function children = getChildren(node,maxNodes,children)
            % Recusively computes the nodes that should be killed
            % deadNodes = getDeadNodes(2,15,[])
            if node > maxNodes
                return
            else
                children = [children,node,...
                    ExpTree.getChildren(2*node,maxNodes,children),...
                    ExpTree.getChildren(2*node+1,maxNodes,children)];
            end
        end
        
    end
    
    %% Start of Private Methods
    methods (Access = private)
        function obj = RandomExpTree(obj,depth,fSetMap,tSetMap)
            obj.maxDepth = depth;
            obj.maxNodes = 2^(obj.maxDepth+1)-1;
            obj.functionSet = fSetMap;
            obj.terminalSet = tSetMap;
            
            obj.binaryTree = cell(obj.maxNodes,1);
            
            % Sequentially adding internal nodes
            numInternalNodes= 2^(obj.maxDepth) - 1;        % Up to the leaves
            obj.binaryTree{1} = obj.chooseFunction();
            for node = 2:obj.maxNodes
                if node <= numInternalNodes
                    depth = floor(log2(node))+1;
                    f = obj.chooseFunction(depth);
                else
                    f = obj.chooseTerminal();
                end
                
                % Always adding to the right (odd nodes), since all
                % functions are at least unary.  If not, making sure that
                % the parent function needs two children before adding
                pContent = obj.getParent(node);
                if ~isempty(pContent)
                    if mod(node,2) == 1
                        if ~isempty(pContent)
                            obj.binaryTree{node} = f;
                        end
                    else
                        if nargin(pContent{1}) == 2
                            obj.binaryTree{node} = f;
                        end
                    end
                end
            end
        end
        
        
        function func = chooseFunction(exprTree,depth)
            % func = chooseFunction(exprTree)
            % func = chooseFunction(exprTree,depth)
            %    returns a function according to it's probability
            v = exprTree.functionSet.values;
            f = exprTree.functionSet.keys;
            weights = cell2mat(v);
            if nargin ==1
                func = f(find(rand<cumsum(weights),1,'first'));
            else
                for key = 1:numel(f);
                    if depth <=3
                        if strcmp(f{key},'plus') || strcmp(f{key},'minus')
                            weights(key) = 10;
                        end
                    elseif depth <=4
                        if strcmp(f{key},'cos') || strcmp(f{key},'sin')
                            weights(key) = 10;
                        end
                    else
                        if (strcmp(f{key},'plus') || strcmp(f{key},'minus') || ...
                                strcmp(f{key},'div') || strcmp(f{key},'times'))
                            weights(key) = 10;
                        end
                    end
                end
                weights = weights/sum(weights);
                func = f(find(rand<cumsum(weights),1,'first'));
            end
        end
        
        function terminal = chooseTerminal(exprTree)
            % terminal = chooseTerminal(exprTree)
            %   returns a terminal node according to it's probability
            v = exprTree.terminalSet.values;
            f = exprTree.terminalSet.keys;
            terminal = f(find(rand<cumsum(cell2mat(v)),1,'first'));
        end
        
        function [obj] = getLeft(exprTree,node)
            % [obj] = getLeft(exprTree,node)
            %    Returns the left child, or the empty set if the child does
            %    not exist
            if 2*node <= numel(exprTree.binaryTree)
                obj = exprTree.binaryTree{2*node};
            else
                warning('exprTree:getLeft','Left child of node %d does not exist',node);
                obj = [];
            end
        end
        function [obj] = getRight(exprTree,node)
            % [obj] = getRight(exprTree,node)
            %   Returns the right child, or the empty set if the child does
            %   not exist.
            if 2*node+1 <= numel(exprTree.binaryTree)
                obj = exprTree.binaryTree{2*node+1};
            else
                warning('exprTree:getRight','Right child of node %d does not exist',node);
                obj = [];
            end
        end
        
        function [obj] = getParent(exprTree,node)
            % [obj] = getParent(exprTree,node)
            %    Returns the parent of a node. If the node is the head node,
            %    the head node is returned.
            if node == 1
                obj = exprTree.binaryTree{node};
            else
                obj = exprTree.binaryTree{floor(node/2)};
            end
        end
        
    end
end
