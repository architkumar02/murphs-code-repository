function children = Child(node,maxNodes,children)
% Recusively computes the nodes that should be killed
% deadNodes = getDeadNodes(2,15,[])
if node > maxNodes
    return
else
    children = [children,node,...
        Child(2*node,maxNodes,children),...
        Child(2*node+1,maxNodes,children)];
end
end

