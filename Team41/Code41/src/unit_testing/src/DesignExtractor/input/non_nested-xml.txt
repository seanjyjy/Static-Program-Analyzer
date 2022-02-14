<program>
	<procedure name=main>
		<stmts>
			<read>
				<var name=a></var>
			</read>
			<assign>
				<var name=b></var>
				<plus>
					<var name=c></var>
					<const val=1></const>
				</plus>
			</assign>
			<while>
				<ne>
					<var name=d></var>
					<const val=1></const>
				</ne>
				<stmts>
					<assign>
						<var name=e></var>
						<const val=2></const>
					</assign>
					<assign>
						<var name=f></var>
						<const val=4></const>
					</assign>
				</stmts>
			</while>
			<if>
				<and>
					<gt>
						<var name=g></var>
						<const val=1></const>
					</gt>
					<eq>
						<var name=h></var>
						<var name=i></var>
					</eq>
				</and>
				<stmts>
					<assign>
						<var name=j></var>
						<const val=3></const>
					</assign>
				</stmts>
				<stmts>
					<print>
						<var name=k></var>
					</print>
					<read>
						<var name=m></var>
					</read>
				</stmts>
			</if>
			<print>
				<var name=n></var>
			</print>
		</stmts>
	</procedure>
</program>