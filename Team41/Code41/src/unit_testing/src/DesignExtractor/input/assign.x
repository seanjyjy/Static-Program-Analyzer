<program>
	<procedure name=main>
		<stmts>
			<assign>
				<var name=a></var>
				<const val=1></const>
			</assign>
			<assign>
				<var name=b></var>
				<var name=c></var>
			</assign>
			<assign>
				<var name=d></var>
				<plus>
					<const val=2></const>
					<const val=3></const>
				</plus>
			</assign>
			<assign>
				<var name=e></var>
				<minus>
					<var name=f></var>
					<const val=3></const>
				</minus>
			</assign>
			<assign>
				<var name=g></var>
				<times>
					<const val=4></const>
					<var name=h></var>
				</times>
			</assign>
			<assign>
				<var name=i></var>
				<div>
					<var name=j></var>
					<var name=k></var>
				</div>
			</assign>
			<assign>
				<var name=m></var>
				<plus>
					<minus>
						<const val=5></const>
						<var name=n></var>
					</minus>
					<times>
						<var name=p></var>
						<const val=6></const>
					</times>
				</plus>
			</assign>
			<assign>
				<var name=q></var>
				<plus>
					<minus>
						<times>
							<var name=r></var>
							<const val=7></const>
						</times>
						<const val=8></const>
					</minus>
					<plus>
						<var name=s></var>
						<div>
							<var name=t></var>
							<var name=u></var>
						</div>
					</plus>
				</plus>
			</assign>
		</stmts>
	</procedure>
</program>